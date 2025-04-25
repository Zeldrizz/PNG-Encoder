# micro-benchmark.py
from __future__ import annotations

import subprocess
import time
import pathlib
import tempfile
import shutil
import sys

from dataclasses import dataclass
from typing import Callable, Dict, List, Tuple

GREEN = "\033[92m"
RED = "\033[91m"
RESET = "\033[0m"
HR = "─" * 90

ROOT_DIR = pathlib.Path(__file__).resolve().parent
RAW_DIR = ROOT_DIR / "examples" / "raw"
PNG_DIR = ROOT_DIR / "examples" / "png"
ENCODER_PATH = ROOT_DIR / "build" / "png_encoder"


def human_readable_bytes(value: int) -> str:
    return f"{value:,}".replace(",", " ")


def run_command(cmd: str) -> None:
    subprocess.check_call(cmd, shell=True,
                          stdout=subprocess.DEVNULL,
                          stderr=subprocess.DEVNULL)


def measure_time(action: Callable[[], None]) -> float:
    start = time.perf_counter()
    action()
    return time.perf_counter() - start


def encode_with_ours(raw_path: pathlib.Path,
                     width: int,
                     height: int,
                     out_path: pathlib.Path) -> None:
    run_command(f"{ENCODER_PATH} {raw_path} {out_path} {width} {height}")


try:
    from PIL import Image
except ImportError:
    sys.exit("Pillow is not installed!")


def encode_with_pillow(input_png: pathlib.Path,
                       out_path: pathlib.Path) -> None:
    with Image.open(input_png) as img:
        img.save(out_path, "PNG", optimize=True)


HAS_OPENCV = False

try:
    import cv2

    HAS_OPENCV = True

    def encode_with_opencv(input_png: pathlib.Path,
                           out_path: pathlib.Path) -> None:
        img = cv2.imread(str(input_png), cv2.IMREAD_UNCHANGED)
        ok, buf = cv2.imencode(".png", img)
        if not ok:
            raise RuntimeError("cv2.imencode failed")
        out_path.write_bytes(buf.tobytes())

except ImportError:
    pass


@dataclass
class BenchmarkResult:
    size:     int
    duration: float


@dataclass
class BenchmarkCase:
    name:     str
    width:    int
    height:   int
    results:  Dict[str, BenchmarkResult]


def run_benchmark(raw_file: pathlib.Path) -> BenchmarkCase:
    stem = raw_file.stem
    width, height = map(int, stem.split("-")[-1].split("x"))
    png_file = PNG_DIR / f"{stem}.png"

    temp_dir = tempfile.TemporaryDirectory()
    work_dir = pathlib.Path(temp_dir.name)

    def measure_encoder(tag: str,
                        fn: Callable[[], None]) -> BenchmarkResult:
        out_file = work_dir / f"{tag}.png"
        elapsed = measure_time(fn)
        size = out_file.stat().st_size
        return BenchmarkResult(size, elapsed)

    results: Dict[str, BenchmarkResult] = {}

    results["ours"] = measure_encoder(
        "ours",
        lambda: encode_with_ours(raw_file, width, height, work_dir / "ours.png")
    )

    results["pillow"] = measure_encoder(
        "pillow",
        lambda: encode_with_pillow(png_file, work_dir / "pillow.png")
    )

    if HAS_OPENCV:
        results["opencv"] = measure_encoder(
            "opencv",
            lambda: encode_with_opencv(png_file, work_dir / "opencv.png")
        )

    temp_dir.cleanup()
    return BenchmarkCase(stem, width, height, results)


def print_results(cases: List[BenchmarkCase],
                  encoders: Tuple[str, ...]) -> None:
    headers = "│".join(f"{enc:^17}" for enc in encoders)

    print(HR)
    print(f"{'File (WxH)':20}│{headers}")
    print(f"{'':20}│" + "│".join(f"{'Size     Time':^17}" for _ in encoders))
    print(HR)

    totals = {e: 0 for e in encoders}

    for case in cases:
        best_size = min(r.size for r in case.results.values())
        best_time = min(r.duration for r in case.results.values())

        line = f"{case.name:20}│"
        for enc in encoders:
            if enc in case.results:
                result = case.results[enc]
                size_str = human_readable_bytes(result.size)
                time_str = f"{result.duration*1e3:6.1f}ms"

                size_col = (
                    GREEN + f"{size_str:>9}" + RESET
                    if result.size == best_size
                    else RED + f"{size_str:>9}" + RESET
                )
                time_col = (
                    GREEN + f"{time_str:>9}" + RESET
                    if result.duration == best_time
                    else RED + f"{time_str:>9}" + RESET
                )

                cell = f"{size_col} {time_col}"
            else:
                cell = " –".center(17)

            line += f"{cell}│"

        print(line)

        for enc, result in case.results.items():
            totals[enc] += result.size

    print(HR)
    print("Totals (bytes):")
    for enc in encoders:
        total = totals.get(enc, 0)
        if total:
            print(f"  {enc:8}: {human_readable_bytes(total)}")
    print(HR)


def main() -> None:
    if not ENCODER_PATH.exists():
        sys.exit("Encoder binary not found: build project first")

    raw_files = sorted(RAW_DIR.glob("*.raw"))
    if not raw_files:
        sys.exit("No raw files found in examples/raw")

    active = ["ours", "pillow"]
    if HAS_OPENCV:
        active.append("opencv")

    print(
        f"Benchmarking {len(raw_files)} images "
        f"using: {', '.join(active)}\n"
    )

    cases = [run_benchmark(f) for f in raw_files]
    print_results(cases, tuple(active))


if __name__ == "__main__":
    main()
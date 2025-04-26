# generate_raw_from_png.py
import sys
from PIL import Image

def png_to_raw(png_path, raw_path):
    with Image.open(png_path) as img:
        img = img.convert('RGB')
        raw_data = img.tobytes()

        with open(raw_path, 'wb') as f:
            f.write(raw_data)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 generate_raw_from_png.py input.png output.raw")
        sys.exit(1)

    png_to_raw(sys.argv[1], sys.argv[2])
import os
import xml.etree.ElementTree as ET

# === Configuration ===
input_dir = r"C:\Users\Sampreeth Raghu\Downloads\job_693-2025_06_30_11_56_43-cvat for images 1.1"
xml_path = os.path.join(input_dir, "annotations.xml")
output_dir = os.path.join(input_dir, "labels")
image_width = 394  # Set actual width if different
image_height = 403  # Set actual height if different

# Your label mapping
class_map = {
    "bondnormal": 0,
    "stitchnormal": 1,
    "bonddefect": 2,
    "stitchdefect": 3
}

# Ensure output directory exists
os.makedirs(output_dir, exist_ok=True)

# === Helper: Convert box to YOLO format ===
def convert_box_yolo(box, img_w, img_h):
    xtl = float(box.get("xtl"))
    ytl = float(box.get("ytl"))
    xbr = float(box.get("xbr"))
    ybr = float(box.get("ybr"))

    cx = (xtl + xbr) / 2.0 / img_w
    cy = (ytl + ybr) / 2.0 / img_h
    w = (xbr - xtl) / img_w
    h = (ybr - ytl) / img_h

    return cx, cy, w, h

# === Parse XML ===
if not os.path.exists(xml_path):
    print(f"❌ XML not found: {xml_path}")
    exit()

tree = ET.parse(xml_path)
root = tree.getroot()

image_tags = root.findall("image")
print(f"📄 Found {len(image_tags)} images in XML")

for image_tag in image_tags:
    image_name = image_tag.get("name")  # e.g., Image (111).png
    if not image_name:
        print("⚠️ Skipping image with no name")
        continue

    base_name = os.path.splitext(image_name)[0]  # Remove .png
    output_path = os.path.join(output_dir, base_name + ".txt")

    boxes = image_tag.findall("box")
    if not boxes:
        continue

    with open(output_path, "w") as f:
        for box in boxes:
            label = box.get("label")
            if label not in class_map:
                print(f"⚠️ Unknown label '{label}', skipping")
                continue
            class_id = class_map[label]
            cx, cy, w, h = convert_box_yolo(box, image_width, image_height)
            f.write(f"{class_id} {cx:.6f} {cy:.6f} {w:.6f} {h:.6f}\n")

print("✅ Done. YOLO label files saved in:", output_dir)

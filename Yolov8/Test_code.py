from ultralytics import YOLO
import os
from glob import glob
import csv
import cv2
import time

def detect_and_save_results(model_path, images_folder, output_folder):
    if not os.path.exists(model_path):
        print(f"❌ Model file not found: {model_path}")
        return
    if not os.path.exists(images_folder):
        print(f"❌ Images folder not found: {images_folder}")
        return
    os.makedirs(output_folder, exist_ok=True)

    # Start timer
    start_time = time.time()

    model = YOLO(model_path)

    # Define class names and colors (4 classes)
    class_names = ['Good', 'baad']
    class_colors = {
        0: (0, 255, 0),     # Green for bondnormal
       # 1: (255, 255, 0),   # Cyan for stitchnormal
        1: (255, 0, 0),     # Red for bonddefect
      #  3: (255, 0, 255),   # Purple for stitchdefect
    }

    image_files = []
    for ext in ('*.jpg', '*.jpeg', '*.png', '*.bmp'):
        image_files.extend(glob(os.path.join(images_folder, ext)))

    if len(image_files) == 0:
        print(f"⚠️ No images found in {images_folder}")
        return

    # CSV to store detection data
    csv_path = os.path.join(output_folder, 'detection_results.csv')
    with open(csv_path, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['image_name', 'class_id', 'class_name', 'confidence', 'x_min', 'y_min', 'x_max', 'y_max'])

        for img_path in image_files:
            print(f"📷 Processing: {img_path}")
            img = cv2.imread(img_path)

            # 👇 Run inference
            results = model(img, imgsz=640, conf=0.3, iou=0.4)

            boxes = results[0].boxes
            if boxes is not None and len(boxes) > 0:
                for box in boxes:
                    cls_id = int(box.cls[0])
                    conf = float(box.conf[0])
                    x_min, y_min, x_max, y_max = map(int, box.xyxy[0])

                    # Get color and class label
                    color = class_colors.get(cls_id, (255, 255, 255))  # fallback white
                    label = f"{class_names[cls_id]} {conf:.2f}"

                    # Draw bounding box and label
                    cv2.rectangle(img, (x_min, y_min), (x_max, y_max), color, 2)
                    #cv2.putText(img, label, (x_min, y_min - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 1)

                    # Save to CSV
                    writer.writerow([
                        os.path.basename(img_path), cls_id, class_names[cls_id],
                        round(conf, 4), x_min, y_min, x_max, y_max
                    ])

            # Save annotated image
            save_path = os.path.join(output_folder, os.path.basename(img_path))
            cv2.imwrite(save_path, img)

    total_time = time.time() - start_time
    print(f"\n✅ Detection completed in {total_time:.2f} seconds.")
    print(f"📁 Results saved in: {output_folder}")


if __name__ == "__main__":
    model_path = r"D:\Kanna Reddinagesh\AI training\Codes\Yolov8\runs\yolov8_custom_model12\weights\best.pt"
    images_folder = r"D:\Kanna Reddinagesh\AI training\Images\Training_sample_1505"
    output_folder = r"D:\Kanna Reddinagesh\AI training\Images\Training_sample_1505\output"

    detect_and_save_results(model_path, images_folder, output_folder)

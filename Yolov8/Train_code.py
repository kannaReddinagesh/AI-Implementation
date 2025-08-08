from ultralytics import YOLO
import torch
import os
import shutil
from sklearn.model_selection import train_test_split
from glob import glob
import cv2
import numpy as np

torch.cuda.empty_cache()

def prepare_dataset(base_path, img_dir, label_dir):
    for split in ['train', 'val', 'test']:
        os.makedirs(os.path.join(img_dir, split), exist_ok=True)
        os.makedirs(os.path.join(label_dir, split), exist_ok=True)

    all_images = glob(os.path.join(img_dir, '*.jpg')) + \
                 glob(os.path.join(img_dir, '*.png')) + \
                 glob(os.path.join(img_dir, '*.bmp'))

    train_imgs, temp_imgs = train_test_split(all_images, test_size=0.3, random_state=42)
    val_imgs, test_imgs = train_test_split(temp_imgs, test_size=1/3, random_state=42)

    def move_files(images, target_img_folder, target_label_folder):
        for img_path in images:
            filename = os.path.basename(img_path)
            label_file = os.path.splitext(filename)[0] + ".txt"
            shutil.copy(img_path, os.path.join(target_img_folder, filename))
            src_label_path = os.path.join(label_dir, label_file)
            dst_label_path = os.path.join(target_label_folder, label_file)
            if os.path.exists(src_label_path):
                shutil.copy(src_label_path, dst_label_path)
            else:
                print(f"⚠️ Warning: No label found for image: {filename}")

    move_files(train_imgs, os.path.join(img_dir, 'train'), os.path.join(label_dir, 'train'))
    move_files(val_imgs, os.path.join(img_dir, 'val'), os.path.join(label_dir, 'val'))
    move_files(test_imgs, os.path.join(img_dir, 'test'), os.path.join(label_dir, 'test'))

    print("✅ Dataset split completed: Train-70%, Val-20%, Test-10%")
    return os.path.join(img_dir, 'test'), os.path.join(label_dir, 'test')


def create_yaml(base_path, class_names):
    yaml_path = os.path.join(base_path, 'data.yaml')
    with open(yaml_path, 'w') as f:
        f.write(f"path: {base_path}\n")
        f.write(f"train: images/train\n")
        f.write(f"val: images/val\n")
        f.write(f"names: {class_names}\n")
    print("📄 YAML file created.")
    return yaml_path


def train_model(yaml_path):
    model = YOLO("yolov8n.pt")

    results = model.train(
        data=yaml_path,
        epochs=50,
        imgsz=640,
        device=0,
        batch=16,
        name="yolov8_custom_model",
        project="runs",
        workers=4,
        conf=0.3,
        iou=0.5,
        hsv_h=0.015,
        hsv_s=0.7,
        hsv_v=0.4,
        degrees=10.0,
        translate=0.1,
        scale=0.5,
        shear=2.0,
        flipud=0.0,
        fliplr=0.5,
        mosaic=1.0,
        mixup=0.0,
        copy_paste=0.0
    )

    print("✅ Training completed.")
    return model


# ✅ 1. Use YOLOv8 Tile Inference (Sliding Window on Large Images)
def tile_inference(model, image_path, tile_size=640, overlap=100, conf_thres=0.3, iou_thres=0.5):
    image = cv2.imread(image_path)
    height, width = image.shape[:2]
    stride = tile_size - overlap
    results_list = []

    for y in range(0, height, stride):
        for x in range(0, width, stride):
            x_end = min(x + tile_size, width)
            y_end = min(y + tile_size, height)
            tile = image[y:y_end, x:x_end]
            result = model.predict(tile, imgsz=tile_size, conf=conf_thres, iou=iou_thres, device=0)[0]

            for box in result.boxes.xyxy.cpu().numpy():
                x1, y1, x2, y2 = box[:4]
                adjusted_box = [x1 + x, y1 + y, x2 + x, y2 + y]
                results_list.append(adjusted_box)

    # Optional: Draw combined boxes
    for box in results_list:
        x1, y1, x2, y2 = map(int, box)
        cv2.rectangle(image, (x1, y1), (x2, y2), (0, 255, 0), 2)

    output_path = image_path.replace(".", "_detected.")
    cv2.imwrite(output_path, image)
    print(f"🖼️ Saved tiled detection result: {output_path}")


def main():
    base_path = 'C:/Users/Sampreeth Raghu/Desktop/AI Implementation/Yolov8/Train_dataset_multi'
    img_dir = os.path.join(base_path, 'images')
    label_dir = os.path.join(base_path, 'labels')
    class_names = ['Good', 'Bad']

    test_img_dir, test_lbl_dir = prepare_dataset(base_path, img_dir, label_dir)
    yaml_path = create_yaml(base_path, class_names)
    model = train_model(yaml_path)

    # ✅ Perform tiled inference on one large test image
    sample_large_image = glob(os.path.join(test_img_dir, '*'))[0]  # pick one image
    tile_inference(model, sample_large_image)


if __name__ == "__main__":
    main()

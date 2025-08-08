from ultralytics import YOLO
import os
import glob

def export_yolo_to_onnx(pt_model_path, onnx_output_path=None, simplify=True, imgsz=640):
    # Check if model exists
    if not os.path.exists(pt_model_path):
        print(f"❌ .pt model not found at: {pt_model_path}")
        return

    # Load the YOLOv8 model
    model = YOLO(pt_model_path)

    # Export to ONNX
    print("📦 Exporting model to ONNX...")
    model.export(
        format="onnx",
        imgsz=imgsz,
        dynamic=False,       # ✅ Fixed here
        simplify=False,
        opset=12            # ✅ Added for DNN compatibility
    )

    # Try to find the most recent ONNX file
    default_output_dir = os.path.dirname(pt_model_path)
    onnx_files = sorted(glob.glob(os.path.join(default_output_dir, "*.onnx")), key=os.path.getmtime, reverse=True)
    
    if onnx_files:
        latest_onnx = onnx_files[0]
        print(f"✅ Exported ONNX model found at: {latest_onnx}")
        
        # Rename/move if a custom output path is provided
        if onnx_output_path and latest_onnx != onnx_output_path:
            os.rename(latest_onnx, onnx_output_path)
            print(f"📁 Moved ONNX model to: {onnx_output_path}")
    else:
        print("⚠️ Export might have succeeded, but no ONNX file was detected.")

if __name__ == "__main__":
    pt_model_path = r"D:\Kanna Reddinagesh\AI training\Codes\Yolov8\runs\yolov8_custom_model12\weights\best.pt"
    onnx_output_path = r"D:\Kanna Reddinagesh\AI training\Codes\Yolov8\runs\yolov8_custom_model12\weights\best.onnx"
    export_yolo_to_onnx(pt_model_path, onnx_output_path)

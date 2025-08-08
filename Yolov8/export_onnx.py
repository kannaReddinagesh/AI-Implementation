from ultralytics import YOLO

# Load trained model
model = YOLO(r"D:\Kanna Reddinagesh\AI training\Codes\Yolov8\runs\yolov8_custom_model1\weights\last.pt")

# Print number of classes and class names
print("Number of classes:", model.model.nc)
print("Class names:", model.model.names)

# Export to ONNX
model.export(format='onnx', simplify=True)

import cv2
import os
import numpy as np

# Folder containing registered faces
face_data_folder = "registered_faces"

# IP camera URL
ip_camera_url = "http://your.url"

# Load the face recognizer
recognizer = cv2.face.LBPHFaceRecognizer_create()

# Initialize face detector
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Prepare training data
labels = []
faces = []
label_id = 0
name_mapping = {}

for filename in os.listdir(face_data_folder):
    filepath = os.path.join(face_data_folder, filename)
    if not filename.endswith(".jpg"):
        continue

    try:
        face_image = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)
        if face_image is None:
            print(f"Error: Unable to load image {filepath}")
            continue
        faces.append(face_image)
        labels.append(label_id)
        name_mapping[label_id] = os.path.splitext(filename)[0]
        label_id += 1
    except Exception as e:
        print(f"Error loading image {filepath}: {e}")

# Train the recognizer
recognizer.train(faces, np.array(labels))
print("Training complete. Recognizer ready.")

# Connect to the IP camera
cap = cv2.VideoCapture(ip_camera_url)

if not cap.isOpened():
    print(f"Error: Could not connect to IP camera at {ip_camera_url}.")
    exit()

print("Press 'q' to quit.")

while True:
    ret, frame = cap.read()
    if not ret:
        print("Error: Failed to capture frame from IP camera.")
        break

    # Convert to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = cv2.equalizeHist(gray)

    # Detect faces
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30), flags=cv2.CASCADE_SCALE_IMAGE)

    for (x, y, w, h) in faces:
        face_roi = gray[y:y + h, x:x + w]

        # Predict the face
        label, confidence = recognizer.predict(face_roi)
        confidence_threshold = 70  # Adjust this value based on your testing
        name = name_mapping.get(label, "Unknown") if confidence < confidence_threshold else "Unknown"

        # Draw rectangle and label
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.putText(frame, f"{name} ({confidence:.2f})", (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (255, 255, 255), 2)

    # Display the frame
    cv2.imshow("Face Recognition", frame)

    # Quit when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
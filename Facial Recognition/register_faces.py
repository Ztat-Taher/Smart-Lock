import cv2
import os

# Folder to save faces
face_data_folder = "registered_faces"
os.makedirs(face_data_folder, exist_ok=True)

# Initialize the webcam
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

print("Press 'c' to capture a face and 'q' to quit.")

while True:
    # Capture a frame
    ret, frame = cap.read()
    if not ret:
        print("Error: Could not capture frame.")
        break

    # Convert to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30), flags=cv2.CASCADE_SCALE_IMAGE)

    # Draw rectangles around faces
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # Display the frame
    cv2.imshow("Face Registration", frame)

    # Capture face when 'c' is pressed
    key = cv2.waitKey(1) & 0xFF
    if key == ord('c') and len(faces) > 0:
        name = input("Enter the name of the person: ").strip()
        if not name.isalnum():
            print("Invalid name. Please use alphanumeric characters only.")
            continue
        for (x, y, w, h) in faces:
            face_image = gray[y:y + h, x:x + w]
            save_path = os.path.join(face_data_folder, f"{name}.jpg")
            cv2.imwrite(save_path, face_image)
            print(f"Saved face for {name} at {save_path}.")

    # Quit when 'q' is pressed
    if key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
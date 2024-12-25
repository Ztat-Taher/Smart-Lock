# Face Registration Script 📝  

### Overview  
This script captures and saves user faces for authentication.  
The faces are stored in the `registered_faces` folder.  

### Features  
- 📸 Live webcam feed for face detection.  
- 💾 Save detected faces with user-defined labels.  

### Usage  
1. Run the script: `python register_faces.py`.  
2. Press `c` to capture a face and assign a name.  
3. Press `q` to quit the application.  

# Facial Recognition Script 🤖 

### Overview  
This script recognizes registered faces in real-time and provides access control.  

### Features  
- 🧠 Pre-trained model for facial recognition.  
- 🚨 Outputs user identification and confidence levels.  
- 🎥 Supports IP camera and local webcam streams.  

### Usage  
1. Run the script: `python facial_recognition.py`.  
2. Ensure registered faces are present in the `registered_faces` folder.  
3. View real-time recognition results in the live feed.  

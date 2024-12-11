# SIOT-KiAI
The main repository for Sensing and the Internet of Things project - KiAI: An IoT Based Kendo Training Assistant for Real-Time Motion Analysis, by Beam Suwiwatchai.

[Youtube](https://youtu.be/1dko0bqK9uo)

## Abstract
Kendo, a traditional Japanese martial art emphasising the unity of spirit, sword, and body (*Ki-Ken-Tai-Ichi*), requires practitioners to develop precise coordination and control. However, refining these skills often depends on expert guidance, which may not always be accessible. This means that solo-practitioners lack tools that offers personalised, driven insights, particularly regarding motion smoothness and classification of kendo techniques. This gap highlights the need for a system that can provide actionable feedback while respecting kendo's principles of focus and discipline. 

This report introduces **KiAI**, an Internet of Things (IoT)-based kendo training assistant that integrates motion sensors, environmental monitors, and real-time video feedback. The system uses MPU9250 sensor to collect acceleration and gyroscope data at 1Hz, combined with machine learning techniques (Support Vector Classifier) to classify kendo moves and assess performance metrics such as motion smoothness, jerk, and acceleration.

Our classification model, trained on four key kendo techniques (*men*, *kote*, *do*, *kamae*), achieved an accuracy of 87.5\% on unseen test data. The results demonstrate that IoT and machine learning can effectively analyse and enhance kendo practice, providing users with data-driven feedback to refine their technique. Future developments, such as integrating expert-labelled datasets and exploring competition-compatible tools like image recognition, could further advance kendo training while preserving its core philosophies.

---

## Project Structure
1. Sensing: Arduino and Python scripts used to collect and store data (both locally and on cloud)
2. Data Analysis: Classfication Model of Kendo Moves
3. Visualisation: Streamlit scripts for running locally
4. [On-Cloud System Deployment Repository](https://github.com/beam-su/KendoAI)

---

## File Explainations
Here's the breakdown of each file in this repository and its purpose:

### Sensing
- **`Local_Data_Collection.ino`**: Collects MPU9250 data and send it through COMS
- **`export_data.py`**: Reads the data from COMS and export to csv
- **`Cloud_Data_Collection.ino`**: Collects MPU9250 data and upload to InfluxDB bucket
- **`Environment_data_collection.ino`**: Collects noise, temperature, and humidity data sends to COMS
- **`data_upload.ino`**: Uploads the environmetal data to InfluxDB bucket
- **`SecretsManager.py`**: Fetch secrets from AWS SecretManager

### Data Analysis
- **`Data Processing.ipynb`**: Jupyter Notebook for data analysis and classification model

### Visualisation
- **`app.py`**: Streamlit webapp for the user dashboard
- - **`requirements.txt.py`**: Required Packages
- **`SecretsManager.py`**: Fetch secrets from AWS SecretManager

---

## Setup Instructions
### Prerequisites
- Hardware:
    - ESP32 DevKitV1 Microcontroller
    - ESP32 CAM + OV2640 camera
    - Arduino Nano Every
    - MPU9250
    - DHT11
    - Microphone
- Software:
    - Arduino IDE with necessary libraries.
    - Python and required packages (see **`requirements.txt`**)

### Installation
1. Clone the repository
```bash
{
git clone https://github.com/beam-su/SIOT-KiAI
}
```

2. Install the dependencies:
```bash
{
pip install -r requirements.txt
}
```

3. Setup the hardware according to the report

4. Run the Streamlit app locally (or use the online webapp):
```bash
{
streamlit run app.py
}
```

---

## Future Work
- Incorporate expert-labelled dataset to improve classification accuracy and capability.
- Explore the use of image recognition for further motion analysis.
- Include more sensors

---
## Acknowledgements
I would like to thank [Dr. David Boyle](https://profiles.imperial.ac.uk/david.boyle) and members of the [Systems & Algorithms Lab](https://www.imperial.ac.uk/systems-algorithms-design-lab/) at Imperial College London for their guidance throughout the project.
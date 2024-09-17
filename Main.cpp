#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <csstudio> // for remove
#include <thread>
#include <chrono>

using namespace std;

// Function to perform HTTP Post requests using libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// send audio file for STT (whisper)
string sendAudioToWhisper(const string& apiKey, const string& audioFilePath) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: multipart/form-data");

        curl_mime* form = curl_mime_init(curl);
        curl_mimepart* field = curl_mime_addpart(form);
        curl_mime_name(field, "file");
        curl_mime_filedata(field, audioFilePath.c_str());

        string url = "https://api.openai.com/v1/audio/transcriptions";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_mime_free(form);
        curl_slist_free_all(headers);
    }
    return readBuffer;
}

int main() {
    string apiKey = "YOUR_OPENAI_API_KEY"; // Replace with your API key
    string audioFilePath = "path_to_your_audio_file.wav"; // Replace with your path to the audio file

    string wakeWord = "Hey AI";  // Example wake word

    // Simulate continuous listening loop
    bool wakeWordDetected = false;

    while (!wakeWordDetected) {
        // Step 1: Send a short segment of audio to Whisper to check for the wake word
        string response = sendAudioToWhisper(apiKey, audioFilePath);
        auto jsonResponse = nlohmann::json::parse(response);
        string transcript = jsonResponse["text"];

        cout << "Transcribed Text: " << transcript << endl;

        // Step 2: Detect wake word in the transcribed text
        if (detectWakeWord(transcript, wakeWord)) {
            cout << "Wake word detected! Ready to listen!" << endl;
            wakeWordDetected = true;
        }
        else {
            cout << "Wake word not detected, listening again...very subtly..." << endl;
        }

        // Sleep to simulate continuous audio capture
        this_thread::sleep_for(chrono::seconds(3));  // Wait a bit before rechecking
    }

    // Step 1: Send audio file to Whisper API to get the transcribed text
    string response = sendAudioToWhisper(apiKey, audioFilePath);

    // Parse the JSON response to extract the transcribed text
    auto jsonResponse = nlohmann::json::parse(response);
    string transcript = jsonResponse["text"];

    cout << "Transcribed Text: " << transcript << endl;

    // Step 2: Send the transcribed text to GPT for further conversation
    string gptResponse = sendTextToGPT(apiKey, transcript);

    // Parse the JSON response from GPT
    auto gptJsonResponse = nlohmann::json::parse(gptResponse);
    string gptReply = gptJsonResponse["choices"][0]["message"]["content"];

    cout << "GPT Response: " << gptReply << endl;

    // Step 3: Delete the audio file after processing
    if (remove(audioFilePath.c_str()) == 0) {
        cout << "Audio file deleted successfully." << endl;
    }
    else {
        cerr << "Error deleting audio file!" << endl;
    }

    return 0;
}

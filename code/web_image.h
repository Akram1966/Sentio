#pragma once
#include "esphome.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <TJpg_Decoder.h>
#include <unistd.h> 

// ---------------------------------------------------------
// 🔧 LINKER FIX: Dummy rmdir implementation
// ---------------------------------------------------------
extern "C" {
  int rmdir(const char *path) {
      return -1; 
  }
}

// ---------------------------------------------------------
// 📷 VIDEO DRIVER
// ---------------------------------------------------------
namespace sentio_video {

    static esphome::display::Display *global_display_ref = nullptr;

    // 1. Pixel Drawer Callback
    static bool tjpg_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
       if (global_display_ref == nullptr) return 0;
       if (y >= global_display_ref->get_height()) return 0;
       
       for (int16_t j = 0; j < h; j++) {
         for (int16_t i = 0; i < w; i++) {
           uint16_t c = bitmap[j * w + i];
           
           // --- FIX: RGB565 DECODING ---
           // We disabled byte swapping, so the integer 'c' is now correct.
           // c = RRRRRGGG GGGBBBBB
           
           uint8_t r = (c >> 11) & 0x1F; // Top 5 bits
           uint8_t g = (c >> 5) & 0x3F;  // Middle 6 bits
           uint8_t b = c & 0x1F;         // Bottom 5 bits
           
           // Scale 5/6 bit to 8-bit (0-255)
           r = (r << 3) | (r >> 2);
           g = (g << 2) | (g >> 4);
           b = (b << 3) | (b >> 2);

           global_display_ref->draw_pixel_at(x + i, y + j, esphome::Color(r, g, b));
         }
       }
       return 1;
    }

    // 2. Downloader & Decoder
    static void draw_doorbell_feed(esphome::display::Display *display, const char* url) {
        global_display_ref = display;
        
        HTTPClient http;
        http.setTimeout(150); 
        http.begin(url);
        
        int httpCode = http.GET();
        if (httpCode == 200) {
            int len = http.getSize();
            if (len > 0 && len < 50000) {
                if (ESP.getFreeHeap() > len + 1024) {
                    uint8_t *buffer = new uint8_t[len];
                    if (buffer) {
                        WiFiClient *stream = http.getStreamPtr();
                        int pos = 0;
                        while (http.connected() && (len > 0 || len == -1)) {
                            size_t size = stream->available();
                            if (size) {
                                int c = stream->readBytes(buffer + pos, size);
                                if (len > 0) len -= c;
                                pos += c;
                            } else {
                                delay(1);
                            }
                            if (len == 0) break;
                        }
                        
                        // Decode
                        TJpgDec.setJpgScale(1);
                        
                        // 🛑 THE FIX: Disable Byte Swap
                        TJpgDec.setSwapBytes(false); 
                        
                        TJpgDec.setCallback(tjpg_output);
                        TJpgDec.drawJpg(0, 0, buffer, pos);
                        
                        delete[] buffer;
                    }
                }
            }
        }
        http.end();
    }

} // end namespace
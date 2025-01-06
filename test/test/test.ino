#include <Arduino.h>
#include <ESP32Lib.h>      // Bitluni's VGA Library
#include <ESP32Video.h>    // Bitluni's Video Driver
#include <lvgl.h>          // LVGL Graphics Library

// Define VGA graphics settings
const int VGA_RED_PIN   = 18;
const int VGA_GREEN_PIN = 22;
const int VGA_BLUE_PIN  = 21;
const int VGA_HSYNC_PIN = 16;
const int VGA_VSYNC_PIN = 17;

const int screenWidth = 320;
const int screenHeight = 240;
VGA3Bit vga;

// LVGL Display buffer
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[screenWidth * 10];  // Buffer for rendering
static lv_color_t buf2[screenWidth * 10];

// LVGL display driver function
void lvgl_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    int32_t x, y;
    for (y = area->y1; y <= area->y2; y++) {
        for (x = area->x1; x <= area->x2; x++) {
            vga.dot(x, y, color_p->full);  // Draw pixel to VGA
            color_p++;
        }
    }
    lv_disp_flush_ready(disp);  // Signal LVGL that flushing is complete
}

void setup() {
    Serial.begin(115200);
    
    // Initialize VGA Display
    vga.init(VGAMode::MODE320x240,
             VGA_RED_PIN, VGA_GREEN_PIN, VGA_BLUE_PIN,
             VGA_HSYNC_PIN, VGA_VSYNC_PIN);
    vga.RGB(0, 0, 255);
    
    // Initialize LVGL
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenWidth * 10);

    // Set up LVGL display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = lvgl_flush;  // Link VGA with LVGL
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Create a Box (Container)
    lv_obj_t *box = lv_obj_create(lv_scr_act()); // Create a box
    lv_obj_set_size(box, 150, 80);               // Set box size
    lv_obj_align(box, LV_ALIGN_CENTER, 0, 0);   // Center on screen

    // Style the Box (Optional)
    lv_obj_set_style_bg_color(box, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_style_border_width(box, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(box, lv_color_white(), LV_PART_MAIN);

    // Create a Label inside the Box
    lv_obj_t *label = lv_label_create(box);     // Attach label to box
    lv_label_set_text(label, "Hello World");    // Set text
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // Center inside the box
}

void loop() {
    lv_timer_handler();  // Handle LVGL tasks
    delay(5);
}

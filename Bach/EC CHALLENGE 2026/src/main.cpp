#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <ESP32Servo.h>
#include "drive.h"
#include "Gripper.h"
#include "LineBot.h"

// --- CẤU HÌNH CHÂN KẾT NỐI ---
#define LIN1 25
#define LIN2 33
#define RIN1 26
#define RIN2 27
#define PWML 32
#define PWMR 14

#define SERVO_PIN1 16
#define SERVO_PIN2 5

// #define ENAL VP
// #define ENBL VN
#define ENAR 34
#define ENBR 35

// --- KHAI BÁO ĐỐI TƯỢNG VÀ BIẾN TOÀN CỤC ---
Gripper GR;
Motor L, R;

uint8_t base_speed = 160;

unsigned long thoiGianCho = 0;
int trangThaiToHop = 0; 
// 0: Rảnh rỗi
// 1: Đang chờ nâng (sau khi đã gắp)
// 2: Đang chờ nhả (sau khi đã hạ)

bool choPhepHoatDong = false;       
bool trangThaiStartTruocDo = false; 

void setup() {
  Serial.begin(115200);
  
  // Khởi tạo Dabble Bluetooth
  Dabble.begin("ESP32_Robot");
  GR.init(SERVO_PIN1, 0, 50 ,SERVO_PIN2, 1, 10);
  // Cấu hình chân động cơ
  // L.init(LIN1, LIN2, PWML);
  // R.init(RIN1, RIN2, PWMR);
  Serial.println("Hệ thống sẵn sàng!");
}

// ==========================================
// CÁC HÀM XỬ LÝ CHỨC NĂNG
// ==========================================

void xuLyDiChuyen() {
  if (GamePad.isUpPressed()){
    L.motor_run(base_speed);
    R.motor_run(base_speed);
    Serial.println("Di thang.");
  }

  else if (GamePad.isDownPressed()){
    L.motor_run(-base_speed);
    R.motor_run(-base_speed);
    Serial.println("Di lui.");
  }

  else if (GamePad.isLeftPressed()){
    L.motor_run(base_speed);
    R.motor_run(-base_speed);
    Serial.println("Quay trai.");
  }

  else if (GamePad.isRightPressed()){
    L.motor_run(-base_speed);
    R.motor_run(base_speed);
    Serial.println("Quay phai.");
  }
  else {
    L.motor_run(0);
    R.motor_run(0);
    Serial.println("Waiting!");
  }
}

void xuLyTayGap() {
  if (GamePad.isCirclePressed()) {
    GR.close(85);
    Serial.println("O (Circle) pressed");
    trangThaiToHop = 1;
    
  }
  
  if (GamePad.isCrossPressed()) {
    GR.open();
    Serial.println("X (Cross) pressed");
    trangThaiToHop = 0;
  }

  if (GamePad.isTrianglePressed()) {
    // GR.close_and_lift(85);
    GR.lift_up();
    Serial.println("Triangle pressed");
    trangThaiToHop = 2;
  }

  if (GamePad.isSquarePressed()) {
    // GR.lift_and_open();
    GR.lift_down();
    Serial.println("Square pressed");
    trangThaiToHop = 0;
  }
}
void loop() {
  Dabble.processInput();
  
  // --- XỬ LÝ NÚT START (BẬT/TẮT) ---
  bool trangThaiStartHienTai = GamePad.isStartPressed();
  
  if (trangThaiStartHienTai == true && trangThaiStartTruocDo == false) {
     choPhepHoatDong = !choPhepHoatDong; 
     
     if (choPhepHoatDong) {
       Serial.println("ĐÃ MỞ KHÓA - Có thể điều khiển!");
     } else {
       Serial.println("ĐÃ KHÓA - Dừng mọi hoạt động.");
     }
  }
  trangThaiStartTruocDo = trangThaiStartHienTai;

  // --- PHÂN LUỒNG ĐIỀU KHIỂN ---
  if (choPhepHoatDong == true) {
    xuLyDiChuyen();
    xuLyTayGap();
  } else {
    //  dieuKhienDongCo(0, 0); // Ép dừng động cơ khi bị khóa
    // L.motor_run(0);
    // R.motor_run(0);
  }
}

#include <SoftwareSerial.h>
// meaningful constants
byte SC_A = 0x04;              byte SC_SPACE = 0x2c;            byte KEY_LEFT_SHIFT = 0x02;
byte SC_B = 0x05;              byte SC_ENTER = 0x58;            byte MOD_CTRL = 0x10;
byte SC_C = 0x06;              byte SC_BCKSPC = 0x2a;
byte SC_D = 0x07;              byte SC_TAB = 0x2b;
byte SC_E = 0x08;              byte SC_ESCAPE = 0x29;
byte SC_F = 0x09;              byte SC_ESC = 0x29;
byte SC_G = 0x0a;              byte SC_RIGHT_ARROW = 0xef;
byte SC_H = 0x0b;              byte SC_LEFT_ARROW = 0x50;
byte SC_I = 0x0c;              byte SC_DOWN_ARROW = 0x51;
byte SC_J = 0x0d;              byte SC_UP_ARROW = 0x52;
byte SC_K = 0x0e;              byte SC_NUM_1 = 0x1e;
byte SC_L = 0x0f;              byte SC_NUM_2 = 0x1f;
byte SC_M = 0x10;              byte SC_NUM_3 = 0x20;
byte SC_N = 0x11;              byte SC_NUM_4 = 0x21;
byte SC_O = 0x12;
byte SC_P = 0x13;
byte SC_Q = 0x14;
byte SC_R = 0x15;
byte SC_S = 0x16;
byte SC_T = 0x17;
byte SC_U = 0x18;
byte SC_V = 0x19;
byte SC_W = 0x1a;
byte SC_X = 0x1b;
byte SC_Y = 0x1c;
byte SC_Z = 0x1d;


SoftwareSerial Input(10, 11); // RX, TX
uint8_t buf[8] = { 0 };
int PinRelay = 5;
int CurrentSeconds = 0;
int SiguienteVuelta = 0;
char *password = "11234";
void setup()
{
  // open hardware serial, TX = 1, RX = 0
  Serial.begin(9600);
  Input.begin(9600);
  delay(1000); // delay 1s to stabilize serial ports
  Input.write("Listo\n");
  pinMode(PinRelay, OUTPUT);
  digitalWrite(PinRelay, HIGH);
}

void loop()
{
  CurrentSeconds = millis() / 1000;
  if (Input.available()) {
    Input.write("Mensaje recibido\n");
    String entrada = Input.readStringUntil('\n');
    char *text = entrada.c_str();
    if (CurrentSeconds > SiguienteVuelta) {
      SiguienteVuelta = CurrentSeconds + 60;
      BorrarRuta();
      AgregarRuta();
      Escribir(text); //OK
      Escribir("+1+1+1+");
      Input.write("Reiniciando letrero.\n");
      digitalWrite(PinRelay, LOW);
      delay(500);
      digitalWrite(PinRelay, HIGH);
    }
  }
}

void Escribir(char *str) {
  ///ya funciona
  Input.write("Escribiendo...");
  Input.write(*str);
  Input.write("\n");

  char *chp = str;
  //delay(5000);
  while (*chp) {
    delay(250);
    if ((*chp >= 'a') && (*chp <= 'z')) {
      buf[2] = *chp - 'a' + 4;
    } else if ((*chp >= 'A') && (*chp <= 'Z')) {
      buf[0] = KEY_LEFT_SHIFT; // Caps
      buf[2] = *chp - 'A' + 4;
    } else {
      switch (*chp) {
        case ' ':
          buf[2] = 0x2c; // Space
          break;
        case '+':
          buf[2] = SC_ENTER;
          break;
        case '*':
          buf[2] = SC_DOWN_ARROW;
          break;
        case '-':
          buf[2] = 0x2d;
          break;
        case '1':
          buf[2] = SC_NUM_1;
          break;
        case '2':
          buf[2] = SC_NUM_2;
          break;
        case '3':
          buf[2] = SC_NUM_3;
          break;
        case '4':
          buf[2] = SC_NUM_4;
          break;
        default:
          /* Character not handled. To do: add rest of chars from HUT1_11.pdf */
          //buf[2] = 0x37; // Period "."
          break;
      }
    }

    Serial.write(buf, 8); // Send keypress
    Input.write(buf, 8);
    buf[0] = 0;
    buf[2] = 0;
    Input.write(buf, 8);
    Serial.write(buf, 8); // Release key
    chp++;
    delay(200);
  }
}


void BorrarRuta() {
  Input.write("Borrando ruta\n");
  Escribir("111234");
  Escribir("+++++");
}

void AgregarRuta() {
  Input.write("Agregando Ruta\n");
  Escribir(password);
  Escribir("++**++");
}

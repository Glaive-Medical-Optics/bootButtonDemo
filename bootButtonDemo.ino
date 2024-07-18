/* 
 *    This demo program shows how to check if the boot button has been pressed
 *    on an Arduino Giga R1 board.
 *    
 *    Author: Glaive Software Group
 *    Revised: 7/16/2024.
 *    
 *    After running this program, each time you click the boot button,
 *    the message "Boot button has been clicked." will appear on the Serial Monitor.
 *    "Click" means press and release.
 *    
 *    After the first click, the red LED comes on.
 *    On the next click, the green LED comes on instead.
 *    On the third click, the blue LED comes on instead.
 *    After another click, the red LED comes on instead and the cycle repeats thereafter.
 *    
 *    This program is Free Software and has ABSOLUTELY NO WARRANTY.
 */

typedef enum
{
    UP,
    DOWN
} ButtonState;

enum
{
  RED_LED,
  GREEN_LED,
  BLUE_LED
};

int LEDpin[3] = {LEDR, LEDG, LEDB};
char *LEDcolor[3] = {"red","green","blue"};

int LEDstate = BLUE_LED;  // initially, pretend the blue LED is lit, so that the next state will be red.

int currentPin = LEDB;

ButtonState currentButtonState = UP;

void setup()
{
  // Initialize the serial communication
  Serial.begin(9600);
      while (!Serial) ; // Wait for the serial port to connect.
  Serial.println("Press the boot button...");

  // Enable the LED pins. They are initially off.

  pinMode(LEDR, OUTPUT);    // red
  pinMode(LEDG, OUTPUT);    // green
  pinMode(LEDB, OUTPUT);    // blue

  // On the Arduino Giga R1 board, the boot button is monitored by GPIO pin PC_13.

  // Enable the clock for port GPIOC.

  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;

  // Configure pin PC_13 as an input pin.

  GPIOC->MODER &= ~(GPIO_MODER_MODE13); // Clear the mode bits to put the pin in input mode.
}

void loop()
{
  
  // Read the state of the boot button
  int pinState = GPIOC->IDR & GPIO_IDR_ID13;

  // Check the state of the boot button.
  // Pin LOW means the button is up; HIGH means the button is down.
  
  if (pinState == LOW)
  {
      // boot button is up
      if (currentButtonState == DOWN)
      {
          Serial.print("BOOT button has been clicked. ");
          currentButtonState = UP;

          // Advance to the next color state

          digitalWrite(currentPin, HIGH);   // Turn off previous color.
          LEDstate++;
          if (LEDstate > BLUE_LED) LEDstate = RED_LED;
          currentPin = LEDpin[LEDstate];
          digitalWrite(currentPin, LOW);   // Light up this pin.

          Serial.print("The ");
          Serial.print(LEDcolor[LEDstate]);
          Serial.println(" LED should now be lit.");
      }
  }
  else // pinState = HIGH
  {
      // boot button is down
      if (currentButtonState == UP)
      {
          currentButtonState = DOWN;
      }
  }
}

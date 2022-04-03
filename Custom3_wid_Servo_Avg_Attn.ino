// Description: Grabs brain data from the serial RX pin and sends CSV out over the TX pin (Half duplex.)
// ALso outputs the data serially over to display
// Custom function for Attention, Servo movement and checks duplicate values
// ALso averages the attention value
// More info: https://github.com/kitschpatrol/Arduino-Brain-Library
// Author: Jatin Vira, 2022

#include <Brain.h>
#include <Servo.h>

#define SET_THRESHOLD 70  //Change the Attention Threshold here


Brain brain(Serial);      // Set up the brain parser, pass it the hardware serial object you want to listen on.
Servo myservo;            // create servo object to control a servo

int pos = 0;              // variable to store the servo position
int i = 0;
int prev_value = 0;       // variable to store the previous attention value
int current_value = 0;    // variable to store the current attention value
const int numReadings = 10;
int Avg_Attention = 0;
int Prev_Avg_Attention = 0;
int valid_attention = 0;
int Avg_Attn_Arr[3] = {0, 0, 0};
int arr_index = 0;

void Calculate_Avg_Attention()
{
  valid_attention = brain.readAttention();
  if (valid_attention >= 10)
  {
    current_value = valid_attention;
    if ( current_value != prev_value)
    {
      Serial.print("1. Valid_Attention is: ");
      Serial.print(valid_attention);
      Serial.println();

      Avg_Attn_Arr[arr_index] = current_value;
      arr_index = arr_index + 1;
      Avg_Attention = (Avg_Attn_Arr[0] + Avg_Attn_Arr[1] + Avg_Attn_Arr[2]) / 3;

      Serial.print("2. Avg_Attention is: ");
      Serial.print(Avg_Attention);
      Serial.println();
      Serial.print("3. Avg_Attn_Arr is: ");
      for (int q = 0; q <= 2; q++)
      {
        Serial.print(Avg_Attn_Arr[q]);
        Serial.print(" ");
      }
      Serial.println();
    }
    if (arr_index == 3)
    {
      arr_index = 0;
    }
    prev_value = current_value;
  }
}

// Function to make Servo motor Move
void Move_Servo()
{
  pos = 0;
  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(1000);                       // waits 15 ms for the servo to reach the position
  pos = 180;
  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(1000);
  Serial.println("MOVED Servo as attention more than ");
  Serial.print(SET_THRESHOLD);
  Serial.println();
}

// Function to Check for attention threshold
void Check_Attention_Threshold()
{
  if (Avg_Attention >= SET_THRESHOLD && Avg_Attention != Prev_Avg_Attention)
  {
    Serial.println("I can doo something");
    Move_Servo();
  }
  Prev_Avg_Attention = Avg_Attention;
}


void setup()
{
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object
  Serial.begin(57600);// Start the hardware serialat 57600 imp
}

void loop()
{
  // Expect packets about once per second.
  // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
  // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"
  if (brain.update())
  {
    //    Serial.println(brain.readErrors());
     //       Serial.println(brain.readCSV());
    if (brain.readSignalQuality() == 0)
    {
//    Serial.println(brain.readCSV());
      Calculate_Avg_Attention();
      Check_Attention_Threshold();
    }
  }
}

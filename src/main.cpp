#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#define SCREENWIDTH 240
#define SCREENHEIGHT 240
#define TFT_BL 25
#define TFT_DC 2
#define height 40
#define width 40
typedef unsigned int uint;
uint cells[height][width] = {0}; // xy
uint row, col;                   //行列
TFT_eSPI tft = TFT_eSPI();
void lifeInit()
{
  for (uint i = 0; i <= height; i++)
  {
    for (uint j = 0; j <= width; j++) //随机初始化为1或0
    {
      cells[i][j] = rand() % 2;
    }
  }
}
void update()
{
  uint neighbour = 0;
  uint newCells[height][width] = {0};
  for (uint i = 0; i < height; i++)
  {
    for (uint j = 0; j < width; j++)
    {
      //计算附近细胞
      if (cells[i] == 0 && cells[j] == 0)
      {
        neighbour = cells[i + 1][j] + cells[j + 1][i] + cells[i + 1][j + 1];
      }
      else if (cells[i] == 0 && cells[j] > 0)
      {
        neighbour = cells[i][j - 1] + cells[i][j + 1] + cells[i + 1][j - 1] + cells[i + 1][j] + cells[i + 1][j + 1];
      }
      else if (cells[j] == 0 && cells[i] > 0)
      {
        neighbour = cells[j][i - 1] + cells[j][i + 1] + cells[j + 1][i - 1] + cells[j + 1][i] + cells[j + 1][i + 1];
      }
      else // (cells[i] > 0 && cells[j] > 0)
      {
        neighbour = cells[i - 1][j + 1] + cells[i][j + 1] + cells[i + 1][j + 1] + cells[i - 1][j] + cells[i + 1][j] + cells[i - 1][j - 1] + cells[i][j - 1] + cells[i + 1][j - 1];
      }
      //****

      //计算迭代
      if ((neighbour == 2 || neighbour == 3) && (cells[i][j] == 1))
      {
        newCells[i][j] = cells[i][j]; //保持上一代
      }
      else if (neighbour == 3)
      {
        newCells[i][j] = 1; //细胞繁殖
      }
      else
      {
        newCells[i][j] = 0; //细胞死亡
      }
      //****
    }
  }
  //更新
  for (uint i = 0; i < height; i++)
  {
    for (uint j = 0; j < width; j++)
    {
      cells[i][j] = newCells[i][j]; //更新下一代数据
    }
  }
  //****
}
void show()
{
  row = col = 0;
  for (uint i = 0; i <= height; i++)
  { // x
    for (uint j = 0; j <= width; j++)
    { // y
      if (cells[i][j] == 1)
      { //为活细胞
        tft.fillRect(row, col, 5, 5, TFT_BLUE);
        row += 6;
      }
      else
      { //为死细胞
        tft.fillRect(row, col, 5, 5, TFT_BLACK);
        row += 6;
      }
    }
    row = 0;                                //换列 行光标置0
    col = col >= SCREENWIDTH ? 0 : col + 6; //换行
  }
}
void setup()
{
  Serial.begin(115200);
  pinMode(TFT_BL,OUTPUT);
  digitalWrite(TFT_BL,LOW);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  lifeInit();
}
void loop()
{
  show();
  update();
  delay(100);
}
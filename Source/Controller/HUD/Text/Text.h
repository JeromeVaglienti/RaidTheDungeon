#ifndef _TEXT_H
#define _TEXT_H

void InitFont();
sfText* InitText(const char* message, const sfVector2f position, const sfColor color, const int size);
void UpdateTextDisplay(sfText* text, const int dataTribe);
void UpdateTextsDisplay(sfText** text, const int dataTribe[], const int index);
void UpdateTextWithStringDisplay(sfText* text, const char* msg, const int data);
void UpdateTextWithSeparator(sfText* text, const int data1, const char* separator, const int data2);
void CenterText(sfText* text, sfVector2f posToCenterTo);
void AddOutline(sfText* text, const float sizeOfOutLine, const sfColor color);
void RescaleText(sfText* text, const float sizeX, const float sizeY, const float ratioX, const float ratioY);
#endif // !_TEXT_H
#ifndef INKRECOGNITION_H
#define INKRECOGNITION_H

#include <QWidget>
#include <windows.h>
#include <msinkaut_i.c>
#include <msinkaut.h>
#include <vector>
#include <memory>
#include <stack>

struct InputRlt {
    char rlt[128];
};

using InPutRltS = std::vector<InputRlt>;

class InkRecognition : public QWidget {
    Q_OBJECT

public:
    explicit InkRecognition(QWidget *parent = nullptr);
    ~InkRecognition();

    void initializeInput(HWND hwnd);
    void recognizeData(InPutRltS &results);
    void releaseResources();
    void clearInk();
    void clearRecognitionData(); // 新增清除识别数据的方法

    std::stack<IInkStrokes*> inkStateStack; // 用于保存笔画状态

private:
    std::unique_ptr<IInkCollector, void(*)(IInkCollector*)> inkCollector;
    std::unique_ptr<IInkDisp, void(*)(IInkDisp*)> inkDisp;
    std::unique_ptr<IInkRecognizerContext, void(*)(IInkRecognizerContext*)> recoContext;
    

    void initializeCOM();
    void cleanupCOM();
};

#endif // INKRECOGNITION_H

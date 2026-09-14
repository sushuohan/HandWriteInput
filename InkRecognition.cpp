#include "inkrecognition.h"
#include <QDebug>
#include <QAxObject>

InkRecognition::InkRecognition(QWidget *parent)
    : QWidget(parent),
    inkCollector(nullptr, [](IInkCollector* ptr) { if (ptr) ptr->Release(); }),
    inkDisp(nullptr, [](IInkDisp* ptr) { if (ptr) ptr->Release(); }),
    recoContext(nullptr, [](IInkRecognizerContext* ptr) { if (ptr) ptr->Release(); }) {
    initializeCOM();
}

InkRecognition::~InkRecognition() {
    releaseResources();
    cleanupCOM();
}

void InkRecognition::initializeCOM() {
    CoInitialize(nullptr);
}

void InkRecognition::cleanupCOM() {
    CoUninitialize();
}

void InkRecognition::initializeInput(HWND hwnd) {
    if (!hwnd) {
        qWarning() << "Invalid HWND provided.";
        return;
    }

    HRESULT hr;

    // 创建识别上下文
    IInkRecognizerContext* tempRecoContext = nullptr;
    hr = CoCreateInstance(CLSID_InkRecognizerContext, nullptr, CLSCTX_INPROC_SERVER,
                          IID_IInkRecognizerContext, reinterpret_cast<void**>(&tempRecoContext));
    if (FAILED(hr)) {
        qWarning() << "With out software";
        return;
    }
    recoContext.reset(tempRecoContext);

    // 创建墨迹收集器
    IInkCollector* tempInkCollector = nullptr;
    hr = CoCreateInstance(CLSID_InkCollector, nullptr, CLSCTX_INPROC_SERVER,
                          IID_IInkCollector, reinterpret_cast<void**>(&tempInkCollector));
    if (FAILED(hr)) {
        qWarning() << "Can not creat ink collection";
        return;
    }
    inkCollector.reset(tempInkCollector);

    // 获取墨迹对象
    IInkDisp* tempInkDisp = nullptr;
    hr = inkCollector->get_Ink(&tempInkDisp);
    if (FAILED(hr)) {
        qWarning() << "Can not get ink objection";
        return;
    }
    inkDisp.reset(tempInkDisp);

    // 关联窗口
    hr = inkCollector->put_hWnd(reinterpret_cast<LONG_PTR>(hwnd));
    if (FAILED(hr)) {
        qWarning() << "Can not connect windows";
        return;
    }

    // 设置默认颜色
    IInkDrawingAttributes* drawingAttributes = nullptr;
    if (SUCCEEDED(inkCollector->get_DefaultDrawingAttributes(&drawingAttributes))) {
        drawingAttributes->put_Color(RGB(0, 0, 255));
        drawingAttributes->Release();
    }

    // 启用墨迹收集器
    hr = inkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr)) {
        qWarning() << "Can not start ink collection";
        return;
    }
}

void InkRecognition::recognizeData(InPutRltS& results) {
	// 检查识别上下文和墨迹对象是否有效
	if (!recoContext || !inkDisp) {
		qWarning() << "The recognition context or inkblot object is not initialized";
		return;
	}

	// 清空结果
	results.clear();

	// 获取墨迹笔触
	IInkStrokes* strokes = nullptr;
	HRESULT hr = inkDisp->get_Strokes(&strokes);
	if (FAILED(hr) || !strokes) {
		qWarning() << "Unable to obtain ink stroke.";
		return;
	}

	// 使用 RAII 管理资源，确保在函数结束时释放
	std::unique_ptr<IInkStrokes, void(*)(IInkStrokes*)> strokesPtr(strokes, [](IInkStrokes* ptr) { if (ptr) ptr->Release(); });

	// 将笔触传递给识别上下文
	hr = recoContext->putref_Strokes(strokes);
	if (FAILED(hr)) {
		qWarning() << "Unable to pass strokes to recognition context.";
		return;
	}

	// 执行识别
	IInkRecognitionResult* recognitionResult = nullptr;
	InkRecognitionStatus status = IRS_NoError;
	hr = recoContext->Recognize(&status, &recognitionResult);
	if (FAILED(hr) || !recognitionResult) {
		qWarning() << "Fail to realize";
		return;
	}

	// 使用 RAII 管理资源，确保在函数结束时释放
	std::unique_ptr<IInkRecognitionResult, void(*)(IInkRecognitionResult*)> recoResultPtr(recognitionResult, [](IInkRecognitionResult* ptr) { if (ptr) ptr->Release(); });

	// 获取可能的识别替代结果
	IInkRecognitionAlternates* alternates = nullptr;
	hr = recognitionResult->AlternatesFromSelection(0, -1, 10, &alternates);
	if (FAILED(hr) || !alternates) {
		qWarning() << "Could not get an alternative for the recognition result.";
		return;
	}

	// 使用 RAII 管理资源，确保在函数结束时释放
	std::unique_ptr<IInkRecognitionAlternates, void(*)(IInkRecognitionAlternates*)> alternatesPtr(alternates, [](IInkRecognitionAlternates* ptr) { if (ptr) ptr->Release(); });

	long count = 0;
	hr = alternates->get_Count(&count);
	if (FAILED(hr) || count <= 0) {
		qWarning() << "No identified alternatives.";
		return;
	}

	count = count < 9 ? count : 9 ;

	// 遍历所有备选项并将结果添加到列表中
	for (long i = 0; i < count; ++i) {
		IInkRecognitionAlternate* alternate = nullptr;
		hr = alternates->Item(i, &alternate);
		if (FAILED(hr) || !alternate) {
			continue; // 如果获取当前备选项失败，跳过
		}

		// 使用 RAII 管理资源，确保在函数结束时释放
		std::unique_ptr<IInkRecognitionAlternate, void(*)(IInkRecognitionAlternate*)> alternatePtr(alternate, [](IInkRecognitionAlternate* ptr) { if (ptr) ptr->Release(); });

		BSTR bstr = nullptr;
		hr = alternate->get_String(&bstr);
		if (SUCCEEDED(hr) && bstr) {
			QString str = QString::fromWCharArray(bstr);
			InputRlt temp = { 0 };
			strcpy(temp.rlt, str.toUtf8().data());
			results.push_back(temp);
			SysFreeString(bstr); // 释放BSTR
		}
	}
	if (results.empty()) {
		qWarning() << "No recognition results.";
		InputRlt temp = { 0 };
		strcpy(temp.rlt, ""); // 默认空字符串
		results.push_back(temp); // 保证至少有一个结果
	}

}


void InkRecognition::clearRecognitionData() {
	// 清除墨迹数据
	clearInk();

	// 释放识别上下文中的笔触引用
	if (recoContext) {
		HRESULT hr = recoContext->putref_Strokes(nullptr); // 解除笔触关联
		if (FAILED(hr)) {
			qWarning() << "Failed to clear strokes from recognition context.";
		}
	}

	qDebug() << "Recognition data cleared successfully.";
}


void InkRecognition::releaseResources() {
    recoContext.reset();
    inkDisp.reset();
    inkCollector.reset();
}

void InkRecognition::clearInk() {
    if (inkDisp) {
        inkDisp->DeleteStrokes(nullptr);
    }
}

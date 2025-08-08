
// MFCApplication1Dlg.cpp : implementation file
//
#include <string>
#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <vector>
#include <opencv2/opencv.hpp>
//#include "onnxruntime_cxx_api.h"

#include <afx.h>
#include <afxwin.h>
//#include <Python.h>  // Add this in your .cpp file

//#include "pch.h"
//#include "MFCApplication1Dlg.h"
//#include <opencv2/opencv.hpp>
//#include <opencv2/dnn.hpp>
//#include <fstream>

//using namespace cv;
//using namespace cv::dnn;

using namespace cv;
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg dialog



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_pictureControl);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnOK();
}


void CMFCApplication1Dlg::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE, _T("*.pt"), NULL, OFN_FILEMUSTEXIST, _T("Model Files (*.pt)|*.onnx||"));
	if (dlg.DoModal() == IDOK)
	{
		m_ModelPath = dlg.GetPathName();
		AfxMessageBox(_T("Model Loaded: ") + m_ModelPath);
	}
	// TODO: Add your control notification handler code here
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE, _T("*.jpg"), NULL, OFN_FILEMUSTEXIST, _T("Image Files (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png||"));
	if (dlg.DoModal() == IDOK)
	{
		m_ImagePath = dlg.GetPathName();
		ShowImage(m_ImagePath);
	}
	// TODO: Add your control notification handler code here
}

void CMFCApplication1Dlg::ShowImage(const CString& path)
{
	CImage image;
	if (FAILED(image.Load(path)))
	{
		AfxMessageBox(_T("Failed to load image!"));
		return;
	}

	CRect rect;
	m_pictureControl.GetClientRect(&rect);

	CDC* pDC = m_pictureControl.GetDC();
	if (pDC == nullptr)
	{
		AfxMessageBox(_T("Failed to get device context!"));
		return;
	}

	image.StretchBlt(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	m_pictureControl.ReleaseDC(pDC);
}

void CMFCApplication1Dlg::OnBnClickedButton3()
{
	std::string modelPath = CT2A(m_ModelPath);
	std::string imagePath = CT2A(m_ImagePath);

	cv::Mat image = cv::imread(imagePath);
	if (image.empty()) {
		AfxMessageBox(_T("❌ Failed to load image!"));
		return;
	}

	cv::dnn::Net net = cv::dnn::readNetFromONNX(modelPath);
	if (net.empty()) {
		AfxMessageBox(_T("❌ Failed to load ONNX model!"));
		return;
	}

	net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

	int inputSize = 640;
	cv::Mat resizedImage;
	cv::resize(image, resizedImage, cv::Size(inputSize, inputSize));

	float xFactor = static_cast<float>(image.cols) / inputSize;
	float yFactor = static_cast<float>(image.rows) / inputSize;

	cv::Mat blob;
	cv::dnn::blobFromImage(resizedImage, blob, 1.0 / 255.0, cv::Size(inputSize, inputSize), cv::Scalar(0, 0, 0), true, false);
	net.setInput(blob);

	std::vector<cv::Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());

	if (outputs.empty()) {
		AfxMessageBox(_T("❌ No output from model!"));
		return;
	}

	cv::Mat& output = outputs[0];  // Expected: [1, 6, 8400] for 2 classes
	if (output.dims != 3 || output.size[1] < 5) {
		AfxMessageBox(_T("❌ Unexpected output shape. Expected [1, 4+Nclasses, N]"));
		return;
	}

	const int numClasses = output.size[1] - 4;
	const int numElements = output.size[2];

	cv::Mat outputMat(output.size[1], numElements, CV_32F, output.ptr<float>());
	cv::Mat transposed;
	cv::transpose(outputMat, transposed); // [8400, 4+numClasses]

	std::vector<cv::Rect> boxes;
	std::vector<float> confidences;
	std::vector<int> classIds;

	const float* data = reinterpret_cast<float*>(transposed.data);
	const float confThreshold = 0.25f;  // Adjust as needed
	const float iouThreshold = 0.3f;

	for (int i = 0; i < numElements; ++i)
	{
		float x = data[i * (4 + numClasses) + 0];
		float y = data[i * (4 + numClasses) + 1];
		float w = data[i * (4 + numClasses) + 2];
		float h = data[i * (4 + numClasses) + 3];

		// Find the class with highest score
		float maxClassScore = -1;
		int maxClassId = -1;

		for (int cls = 0; cls < numClasses; ++cls)
		{
			float classScore = data[i * (4 + numClasses) + 4 + cls];
			if (classScore > maxClassScore)
			{
				maxClassScore = classScore;
				maxClassId = cls;
			}
		}

		if (maxClassScore > confThreshold)
		{
			int cx = static_cast<int>(x * xFactor);
			int cy = static_cast<int>(y * yFactor);
			int width = static_cast<int>(w * xFactor);
			int height = static_cast<int>(h * yFactor);
			int left = cx - width / 2;
			int top = cy - height / 2;

			if (width < 5 || height < 5)
				continue;

			boxes.emplace_back(left, top, width, height);
			confidences.push_back(maxClassScore);
			classIds.push_back(maxClassId);

			CString str;
			str.Format(L"Detection: Class=%d Conf=%.2f Box=[%d, %d, %d, %d]\n", maxClassId, maxClassScore, left, top, width, height);
			OutputDebugString(str);
		}
	}

	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, confThreshold, iouThreshold, indices);

	for (int idx : indices)
	{
		// Map class ID to class name
		CString className;
		if (classIds[idx] == 0) className = _T("Good");
		else if (classIds[idx] == 1) className = _T("baad");
		else className = _T("Unknown");

		// Print to debug log in "Good 0.83" format
		CString str;
		str.Format(L"%s %.4f\n", className.GetString(), confidences[idx]);
		OutputDebugString(str);

		// Draw bounding box (optional visual)
		cv::Scalar color = (classIds[idx] == 0) ? cv::Scalar(0, 255, 0) :
			(classIds[idx] == 1) ? cv::Scalar(0, 0, 255) :
			cv::Scalar(255, 255, 255);
		cv::rectangle(image, boxes[idx], color, 2);
	}

	std::string outputPath = "C:\\Users\\Sampreeth Raghu\\Desktop\\output_with_boxes.jpg";
	cv::imwrite(outputPath, image);
	ShowImage(CString(outputPath.c_str()));

	CString msg;
	msg.Format(_T("%d object(s) found."), (int)indices.size());
	SetDlgItemText(IDC_EDIT1, msg);
}



void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

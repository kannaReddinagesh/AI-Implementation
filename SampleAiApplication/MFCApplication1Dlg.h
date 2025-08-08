
// MFCApplication1Dlg.h : header file
//

#pragma once
#pragma once
#include <vector>
#include <string>
#include <iostream>

//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/dnn.hpp>


// CMFCApplication1Dlg dialog
class CMFCApplication1Dlg : public CDialogEx
{
// Construction
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// standard constructor
	//void RunYOLOInspection_DrawAllBoxes(cv::dnn::Net& net, const cv::Mat& image, const cv::Size& inputSize, const std::string& outputPath);
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CString m_ModelPath;
	CString m_ImagePath;
	CString m_OutputPath;

	
	afx_msg void OnBnClickedButton2();
	void ShowImage(const CString& path);
	CStatic m_pictureControl;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEdit1();
};

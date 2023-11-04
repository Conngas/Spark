#include "spkpch.h"
#include "Spark/Core/Application.h"
#include "Spark/Utils/PlatformUtils.h"

#include <sstream>
#include <commdlg.h>
#include <GLFW/glfw3.h>
// ��ȡ�����ڲ�����
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Spark {
	// Standard Windows OpenFile Option ��׼Win�ļ�������ʹ�ÿ��ַ�����֤������
	std::optional<std::wstring> FileDialogs::OpenFile(const WCHAR* filter)
	{
		// �洢�ļ��Ի���������Ϣ
		OPENFILENAMEW openfileName;
		// �����ļ�·���ַ����
		WCHAR szFile[260] = { 0 };
		// ��ʼ����ԱΪ0
		ZeroMemory(&openfileName, sizeof(OPENFILENAMEW));
		// ���ýṹ��С
		openfileName.lStructSize = sizeof(OPENFILENAMEW);
		// Get glfw nativeWindow ��ȡglfw�������Ա����´��ڲ���ʱ����ԭ����
		openfileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		// �洢�ļ�·��������
		openfileName.lpstrFile = szFile;
		// ���û�������С
		openfileName.nMaxFile = sizeof(szFile);
		// �����ļ�������������ʹ����Щ�ļ����
		openfileName.lpstrFilter = filter;
		// �ļ�����������
		openfileName.nFilterIndex = 1;
		// ֻ���������·�� ֻ��ѡ������ļ� �Ի��򲻸ı䵱ǰ����Ŀ¼
		openfileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		// �򿪱����ļ��Ի��򣬵�������򷵻��ļ�·����W�汾���ڴ���Unicode�ַ������ı�����֧��
		if (GetOpenFileNameW(&openfileName) == TRUE)
		{
			return openfileName.lpstrFile;
		}
		// ȡ���������ؿ�optional�ļ�
		return std::nullopt;
	}

	std::optional<std::wstring> FileDialogs::SaveFile(const WCHAR* filter)
	{
		OPENFILENAMEW openFileName;
		WCHAR szFile[260] = { 0 };
		ZeroMemory(&openFileName, sizeof(OPENFILENAMEW));
		openFileName.lStructSize = sizeof(OPENFILENAMEW);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		openFileName.lpstrFile = szFile;
		openFileName.nMaxFile = sizeof(szFile);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		// δָ������ʱ��ʹ��Ĭ�ϵ���չ���ƴ洢�ļ�
		openFileName.lpstrDefExt = std::wcschr(filter, L'\0') + 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameW(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile;
		}
		return std::nullopt;
	}
}

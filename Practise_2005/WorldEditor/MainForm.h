#pragma once

#include "Matrices.h"


namespace WorldEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 ժҪ
	///
	/// ����: ������Ĵ�������ƣ�����Ҫ����
	///          ����������������� .resx �ļ��������й���Դ���������ߵ�
	///          ����Դ�ļ��������ԡ�����
	///          �������������˴���Ĺ���
	///          ���ػ���Դ��ȷ������
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
			pSystem = new RenderSystem();
			pSystem->InitD3D((HWND)this->Handle.ToPointer());
			pSystem->InitGeometry();
		}

	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
			if (pSystem)
			{
				delete pSystem;
				pSystem = NULL;
			}
		}
	private: System::ComponentModel::IContainer^  components;
	protected: 
	private: System::Windows::Forms::Timer^  timerRender;

	private:
		/// <summary>
		/// ����������������
		/// </summary>

		RenderSystem* pSystem;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����֧������ķ��� - ��Ҫ
		/// ʹ�ô���༭���޸Ĵ˷��������ݡ�
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timerRender = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// timerRender
			// 
			this->timerRender->Enabled = true;
			this->timerRender->Interval = 20;
			this->timerRender->Tick += gcnew System::EventHandler(this, &MainForm::timerRender_Tick);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 273);
			this->Name = L"MainForm";
			this->Text = L"WorldEditor";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void timerRender_Tick(System::Object^  sender, System::EventArgs^  e) {
				 if (pSystem)
				 {
					 pSystem->Render();
				 }
			 }
	};
}


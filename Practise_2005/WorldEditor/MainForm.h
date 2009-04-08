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
	/// Form1 摘要
	///
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			pSystem = new RenderSystem();
			pSystem->InitD3D((HWND)this->Handle.ToPointer());
			pSystem->InitGeometry();
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
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
		/// 必需的设计器变量。
		/// </summary>

		RenderSystem* pSystem;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
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


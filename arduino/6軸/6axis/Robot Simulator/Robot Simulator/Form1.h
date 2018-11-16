#pragma once
#include "Robot.h"

namespace RobotSimulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Riepilogo per Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
		

	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: aggiungere qui il codice del costruttore.
			//
		}

	protected:
		/// <summary>
		/// Liberare le risorse in uso.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::NumericUpDown^  nA1;
	private: System::Windows::Forms::NumericUpDown^  nA6;
	protected:

	protected:

	protected:


	private: System::Windows::Forms::NumericUpDown^  nA5;

	private: System::Windows::Forms::NumericUpDown^  nA4;

	private: System::Windows::Forms::NumericUpDown^  nA3;
	private: System::Windows::Forms::NumericUpDown^  nA2;



	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::NumericUpDown^  nX;
	private: System::Windows::Forms::NumericUpDown^  nC;


	private: System::Windows::Forms::NumericUpDown^  nB;

	private: System::Windows::Forms::NumericUpDown^  nA;

	private: System::Windows::Forms::NumericUpDown^  nZ;

	private: System::Windows::Forms::NumericUpDown^  nY;

	private: System::Windows::Forms::Button^  btMove;


	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Label^  lB;
	private: System::Windows::Forms::Label^  lC;


	private: System::Windows::Forms::Label^  lA;

	private: System::Windows::Forms::Label^  lZ;

	private: System::Windows::Forms::Label^  lY;

	private: System::Windows::Forms::Label^  lX;
	private: System::Windows::Forms::RadioButton^  rdPositions;


	private: System::Windows::Forms::RadioButton^  rAngles;
	public: System::Windows::Forms::PictureBox^  pictureBox1;

	private:
		/// <summary>
		/// Variabile di progettazione necessaria.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metodo necessario per il supporto della finestra di progettazione. Non modificare
		/// il contenuto del metodo con l'editor di codice.
		/// </summary>
		void InitializeComponent(void)
		{
			this->nA1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->nA6 = (gcnew System::Windows::Forms::NumericUpDown());
			this->nA5 = (gcnew System::Windows::Forms::NumericUpDown());
			this->nA4 = (gcnew System::Windows::Forms::NumericUpDown());
			this->nA3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->nA2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->btMove = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->lB = (gcnew System::Windows::Forms::Label());
			this->lC = (gcnew System::Windows::Forms::Label());
			this->lA = (gcnew System::Windows::Forms::Label());
			this->lZ = (gcnew System::Windows::Forms::Label());
			this->lY = (gcnew System::Windows::Forms::Label());
			this->lX = (gcnew System::Windows::Forms::Label());
			this->rdPositions = (gcnew System::Windows::Forms::RadioButton());
			this->rAngles = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->nX = (gcnew System::Windows::Forms::NumericUpDown());
			this->nC = (gcnew System::Windows::Forms::NumericUpDown());
			this->nB = (gcnew System::Windows::Forms::NumericUpDown());
			this->nA = (gcnew System::Windows::Forms::NumericUpDown());
			this->nZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->nY = (gcnew System::Windows::Forms::NumericUpDown());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA2))->BeginInit();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nX))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nC))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nB))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// nA1
			// 
			this->nA1->DecimalPlaces = 2;
			this->nA1->Location = System::Drawing::Point(32, 19);
			this->nA1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, 0 });
			this->nA1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, System::Int32::MinValue });
			this->nA1->Name = L"nA1";
			this->nA1->Size = System::Drawing::Size(47, 20);
			this->nA1->TabIndex = 0;
			this->nA1->ValueChanged += gcnew System::EventHandler(this, &Form1::nA1_ValueChanged);
			// 
			// nA6
			// 
			this->nA6->DecimalPlaces = 2;
			this->nA6->Location = System::Drawing::Point(32, 149);
			this->nA6->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, 0 });
			this->nA6->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, System::Int32::MinValue });
			this->nA6->Name = L"nA6";
			this->nA6->Size = System::Drawing::Size(47, 20);
			this->nA6->TabIndex = 2;
			this->nA6->ValueChanged += gcnew System::EventHandler(this, &Form1::nA6_ValueChanged);
			// 
			// nA5
			// 
			this->nA5->DecimalPlaces = 2;
			this->nA5->Location = System::Drawing::Point(32, 123);
			this->nA5->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, 0 });
			this->nA5->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, System::Int32::MinValue });
			this->nA5->Name = L"nA5";
			this->nA5->Size = System::Drawing::Size(47, 20);
			this->nA5->TabIndex = 3;
			this->nA5->ValueChanged += gcnew System::EventHandler(this, &Form1::nA5_ValueChanged);
			// 
			// nA4
			// 
			this->nA4->DecimalPlaces = 2;
			this->nA4->Location = System::Drawing::Point(32, 97);
			this->nA4->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, 0 });
			this->nA4->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, System::Int32::MinValue });
			this->nA4->Name = L"nA4";
			this->nA4->Size = System::Drawing::Size(47, 20);
			this->nA4->TabIndex = 4;
			this->nA4->ValueChanged += gcnew System::EventHandler(this, &Form1::nA4_ValueChanged);
			// 
			// nA3
			// 
			this->nA3->DecimalPlaces = 2;
			this->nA3->Location = System::Drawing::Point(32, 71);
			this->nA3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, 0 });
			this->nA3->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, System::Int32::MinValue });
			this->nA3->Name = L"nA3";
			this->nA3->Size = System::Drawing::Size(47, 20);
			this->nA3->TabIndex = 5;
			this->nA3->ValueChanged += gcnew System::EventHandler(this, &Form1::nA3_ValueChanged);
			// 
			// nA2
			// 
			this->nA2->DecimalPlaces = 2;
			this->nA2->Location = System::Drawing::Point(32, 45);
			this->nA2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, 0 });
			this->nA2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3600, 0, 0, System::Int32::MinValue });
			this->nA2->Name = L"nA2";
			this->nA2->Size = System::Drawing::Size(47, 20);
			this->nA2->TabIndex = 6;
			this->nA2->ValueChanged += gcnew System::EventHandler(this, &Form1::nA2_ValueChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->nA1);
			this->groupBox1->Controls->Add(this->nA6);
			this->groupBox1->Controls->Add(this->nA5);
			this->groupBox1->Controls->Add(this->nA4);
			this->groupBox1->Controls->Add(this->nA3);
			this->groupBox1->Controls->Add(this->nA2);
			this->groupBox1->Location = System::Drawing::Point(3, 24);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(85, 184);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Angles";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(7, 125);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(19, 13);
			this->label6->TabIndex = 13;
			this->label6->Text = L"a5";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(7, 151);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(19, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"a6";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 97);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(19, 13);
			this->label1->TabIndex = 8;
			this->label1->Text = L"a4";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 71);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(19, 13);
			this->label2->TabIndex = 9;
			this->label2->Text = L"a3";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 47);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(19, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"a2";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(7, 21);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(19, 13);
			this->label4->TabIndex = 11;
			this->label4->Text = L"a1";
			this->label4->Click += gcnew System::EventHandler(this, &Form1::label4_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->btMove);
			this->splitContainer1->Panel1->Controls->Add(this->groupBox3);
			this->splitContainer1->Panel1->Controls->Add(this->rdPositions);
			this->splitContainer1->Panel1->Controls->Add(this->rAngles);
			this->splitContainer1->Panel1->Controls->Add(this->groupBox2);
			this->splitContainer1->Panel1->Controls->Add(this->groupBox1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->pictureBox1);
			this->splitContainer1->Size = System::Drawing::Size(623, 501);
			this->splitContainer1->SplitterDistance = 186;
			this->splitContainer1->TabIndex = 8;
			// 
			// btMove
			// 
			this->btMove->Location = System::Drawing::Point(12, 451);
			this->btMove->Name = L"btMove";
			this->btMove->Size = System::Drawing::Size(75, 23);
			this->btMove->TabIndex = 18;
			this->btMove->Text = L"Move";
			this->btMove->UseVisualStyleBackColor = true;
			this->btMove->Click += gcnew System::EventHandler(this, &Form1::btMove_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->lB);
			this->groupBox3->Controls->Add(this->lC);
			this->groupBox3->Controls->Add(this->lA);
			this->groupBox3->Controls->Add(this->lZ);
			this->groupBox3->Controls->Add(this->lY);
			this->groupBox3->Controls->Add(this->lX);
			this->groupBox3->Location = System::Drawing::Point(12, 261);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(67, 184);
			this->groupBox3->TabIndex = 15;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Actual";
			// 
			// lB
			// 
			this->lB->AutoSize = true;
			this->lB->Location = System::Drawing::Point(7, 125);
			this->lB->Name = L"lB";
			this->lB->Size = System::Drawing::Size(13, 13);
			this->lB->TabIndex = 13;
			this->lB->Text = L"b";
			// 
			// lC
			// 
			this->lC->AutoSize = true;
			this->lC->Location = System::Drawing::Point(7, 151);
			this->lC->Name = L"lC";
			this->lC->Size = System::Drawing::Size(13, 13);
			this->lC->TabIndex = 12;
			this->lC->Text = L"c";
			// 
			// lA
			// 
			this->lA->AutoSize = true;
			this->lA->Location = System::Drawing::Point(7, 97);
			this->lA->Name = L"lA";
			this->lA->Size = System::Drawing::Size(13, 13);
			this->lA->TabIndex = 8;
			this->lA->Text = L"a";
			// 
			// lZ
			// 
			this->lZ->AutoSize = true;
			this->lZ->Location = System::Drawing::Point(6, 71);
			this->lZ->Name = L"lZ";
			this->lZ->Size = System::Drawing::Size(12, 13);
			this->lZ->TabIndex = 9;
			this->lZ->Text = L"z";
			// 
			// lY
			// 
			this->lY->AutoSize = true;
			this->lY->Location = System::Drawing::Point(6, 47);
			this->lY->Name = L"lY";
			this->lY->Size = System::Drawing::Size(12, 13);
			this->lY->TabIndex = 10;
			this->lY->Text = L"y";
			// 
			// lX
			// 
			this->lX->AutoSize = true;
			this->lX->Location = System::Drawing::Point(7, 21);
			this->lX->Name = L"lX";
			this->lX->Size = System::Drawing::Size(12, 13);
			this->lX->TabIndex = 11;
			this->lX->Text = L"x";
			// 
			// rdPositions
			// 
			this->rdPositions->AutoSize = true;
			this->rdPositions->Location = System::Drawing::Point(12, 214);
			this->rdPositions->Name = L"rdPositions";
			this->rdPositions->Size = System::Drawing::Size(67, 17);
			this->rdPositions->TabIndex = 16;
			this->rdPositions->Text = L"Positions";
			this->rdPositions->UseVisualStyleBackColor = true;
			// 
			// rAngles
			// 
			this->rAngles->AutoSize = true;
			this->rAngles->Checked = true;
			this->rAngles->Location = System::Drawing::Point(3, 3);
			this->rAngles->Name = L"rAngles";
			this->rAngles->Size = System::Drawing::Size(57, 17);
			this->rAngles->TabIndex = 15;
			this->rAngles->TabStop = true;
			this->rAngles->Text = L"Angles";
			this->rAngles->UseVisualStyleBackColor = true;
			this->rAngles->CheckedChanged += gcnew System::EventHandler(this, &Form1::rAngles_CheckedChanged);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->label8);
			this->groupBox2->Controls->Add(this->label9);
			this->groupBox2->Controls->Add(this->label10);
			this->groupBox2->Controls->Add(this->label11);
			this->groupBox2->Controls->Add(this->label12);
			this->groupBox2->Controls->Add(this->nX);
			this->groupBox2->Controls->Add(this->nC);
			this->groupBox2->Controls->Add(this->nB);
			this->groupBox2->Controls->Add(this->nA);
			this->groupBox2->Controls->Add(this->nZ);
			this->groupBox2->Controls->Add(this->nY);
			this->groupBox2->Enabled = false;
			this->groupBox2->Location = System::Drawing::Point(85, 261);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(85, 184);
			this->groupBox2->TabIndex = 14;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Position";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(7, 125);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(13, 13);
			this->label7->TabIndex = 13;
			this->label7->Text = L"b";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(7, 151);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(13, 13);
			this->label8->TabIndex = 12;
			this->label8->Text = L"c";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(7, 97);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(13, 13);
			this->label9->TabIndex = 8;
			this->label9->Text = L"a";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(6, 71);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(12, 13);
			this->label10->TabIndex = 9;
			this->label10->Text = L"z";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(6, 47);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(12, 13);
			this->label11->TabIndex = 10;
			this->label11->Text = L"y";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(7, 21);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(12, 13);
			this->label12->TabIndex = 11;
			this->label12->Text = L"x";
			// 
			// nX
			// 
			this->nX->Location = System::Drawing::Point(32, 19);
			this->nX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->nX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, System::Int32::MinValue });
			this->nX->Name = L"nX";
			this->nX->Size = System::Drawing::Size(47, 20);
			this->nX->TabIndex = 0;
			this->nX->ValueChanged += gcnew System::EventHandler(this, &Form1::nX_ValueChanged);
			// 
			// nC
			// 
			this->nC->Location = System::Drawing::Point(32, 149);
			this->nC->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->nC->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, System::Int32::MinValue });
			this->nC->Name = L"nC";
			this->nC->Size = System::Drawing::Size(47, 20);
			this->nC->TabIndex = 2;
			this->nC->ValueChanged += gcnew System::EventHandler(this, &Form1::nC_ValueChanged);
			// 
			// nB
			// 
			this->nB->Location = System::Drawing::Point(32, 123);
			this->nB->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->nB->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, System::Int32::MinValue });
			this->nB->Name = L"nB";
			this->nB->Size = System::Drawing::Size(47, 20);
			this->nB->TabIndex = 3;
			this->nB->ValueChanged += gcnew System::EventHandler(this, &Form1::nB_ValueChanged);
			// 
			// nA
			// 
			this->nA->Location = System::Drawing::Point(32, 97);
			this->nA->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->nA->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, System::Int32::MinValue });
			this->nA->Name = L"nA";
			this->nA->Size = System::Drawing::Size(47, 20);
			this->nA->TabIndex = 4;
			this->nA->ValueChanged += gcnew System::EventHandler(this, &Form1::nA_ValueChanged);
			// 
			// nZ
			// 
			this->nZ->Location = System::Drawing::Point(32, 71);
			this->nZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->nZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, System::Int32::MinValue });
			this->nZ->Name = L"nZ";
			this->nZ->Size = System::Drawing::Size(47, 20);
			this->nZ->TabIndex = 5;
			this->nZ->ValueChanged += gcnew System::EventHandler(this, &Form1::nZ_ValueChanged);
			// 
			// nY
			// 
			this->nY->Location = System::Drawing::Point(32, 45);
			this->nY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->nY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, System::Int32::MinValue });
			this->nY->Name = L"nY";
			this->nY->Size = System::Drawing::Size(47, 20);
			this->nY->TabIndex = 6;
			this->nY->ValueChanged += gcnew System::EventHandler(this, &Form1::nY_ValueChanged);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Location = System::Drawing::Point(0, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(433, 501);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->SizeChanged += gcnew System::EventHandler(this, &Form1::pictureBox1_SizeChanged);
			this->pictureBox1->Click += gcnew System::EventHandler(this, &Form1::pictureBox1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(623, 501);
			this->Controls->Add(this->splitContainer1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA2))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nX))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nC))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nB))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nA))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
		const double pi = 3.14159265359;
		const double dToR = 1.74532925199432E-02;
		Robot^ r;

private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
	}
		 
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			
			 r=gcnew Robot(pictureBox1->Width, pictureBox1->Height);

			 pictureBox1->Image = r->GetImage();
			/* 
			 cbMove->Items->Add("MoveL");
			 cbMove->Items->Add("MoveC");*/

}
private: System::Void btMove_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void pictureBox1_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
			 r->Resize(pictureBox1->Width, pictureBox1->Height);
			 pictureBox1->Image = r->GetImage();

}
		 private:double round(double x, int n)
		 {
					 long v;
					 v = Math::Pow(10, n);
					 return Math::Round(x*v) / v;

		 }
 private:void RefreshLabels()
 {
			 tuple^ p = r->GetPosition();
			 lX->Text = round((p->t1),4).ToString();
			 lY->Text = round(p->t2, 4).ToString();
			 lZ->Text = round(p->t3, 4).ToString();
			 lA->Text = round(p->t4 / dToR, 4).ToString();
			 lB->Text = round(p->t5 / dToR, 4).ToString();
			 lC->Text = round(p->t6 / dToR, 4).ToString();

 }
		private:void RefreshAngles()
		{
					r->MoveToMotorAngles((double)nA1->Value*dToR,
						(double)nA2->Value*dToR,
						(double)nA3->Value*dToR,
						(double)nA4->Value*dToR,
						(double)nA5->Value*dToR,
						(double)nA6->Value*dToR);
					this->pictureBox1->Image = r->GetImage();
					RefreshLabels();
		}
private: System::Void nA1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (rAngles->Checked)
			 RefreshAngles();
}
		 
private: System::Void nA2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (rAngles->Checked)
			 RefreshAngles();
}
private: System::Void nA3_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (rAngles->Checked)
			 RefreshAngles();
}
private: System::Void nA4_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (rAngles->Checked)
			 RefreshAngles();
}
private: System::Void nA5_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (rAngles->Checked)
			 RefreshAngles();
}
private: System::Void nA6_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (rAngles->Checked)
			 RefreshAngles();
}
private: System::Void rAngles_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->groupBox1->Enabled = rAngles->Checked;
			 this->groupBox2->Enabled = !rAngles->Checked;
}
private:void RefreshPositions()
{
			tuple^ p;
	p=r->MoveToPositions((double)nX->Value,
							(double)nY->Value,
								(double)nZ->Value,
								(double)nA->Value*dToR,
								(double)nB->Value*dToR,
								(double)nC->Value*dToR);
	this->pictureBox1->Image = r->GetImage();
	try
	{
	
	nA1->Value=Decimal::Round((Decimal)(p->t1/dToR),2);
	nA2->Value = Decimal::Round((Decimal)(p->t2 / dToR), 2);
	nA3->Value = Decimal::Round((Decimal)(p->t3 / dToR), 2);
	nA4->Value = Decimal::Round((Decimal)(p->t4 / dToR), 2);
	nA5->Value = Decimal::Round((Decimal)(p->t5 / dToR), 2);
	nA6->Value = Decimal::Round((Decimal)(p->t6 / dToR), 2);
	}
	catch (...)
	{
	}
	RefreshLabels();
	}
private: System::Void nX_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 RefreshPositions();

}
private: System::Void nY_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 RefreshPositions();
}
private: System::Void nZ_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 RefreshPositions();
}
private: System::Void nA_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 RefreshPositions();
}
private: System::Void nB_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 RefreshPositions();
}
private: System::Void nC_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 RefreshPositions();
}
};


}


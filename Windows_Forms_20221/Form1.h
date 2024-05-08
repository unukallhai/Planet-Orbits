#pragma once
#include <iostream>
#include "Orbit.h"
#include "consts.h"

namespace Form1_ {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	private:
		BufferedGraphics^ myBuffer;

	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

		void BaseSettings()
		{
			scale = 2000;
			n = (trackBar2->Value)/100;
			dt = 0.02; // в днях

			M1 = 1;
			M2 = 1.5;
			L1 = 1;
			L2 = 1;
			R1 = 1;
			R2 = 2.25;
			Temp1 = SolTemp;
			Temp2 = SolTemp;

			v = -1;

			r_star = 0.02;
			r_planet = 0.05;
		}

		void ShowScale()
		{
			myPen = gcnew Pen(System::Drawing::Color::FromArgb(255, 205, 92, 92), 5);
			char str[128];
			if (scale < (pictureBox1->Width))
			{
				myBuffer->Graphics->DrawLine(myPen, PointF(2.5, 0), PointF(scale, 0));
				myBuffer->Graphics->DrawLine(myPen, PointF(2.5, 0), PointF(2.5, 10));
				myBuffer->Graphics->DrawLine(myPen, PointF(scale, 0), PointF(scale, 10));
				sprintf(str, "Масштаб 1 а.е. : %0.0f пк", scale);
			}
			if (scale >= (pictureBox1->Width))
			{
				myBuffer->Graphics->DrawLine(myPen, PointF(2.5, 0), PointF(scale/10, 0));
				myBuffer->Graphics->DrawLine(myPen, PointF(2.5, 0), PointF(2.5, 10));
				myBuffer->Graphics->DrawLine(myPen, PointF(scale/10, 0), PointF(scale/10, 10));
				sprintf(str, "Масштаб 0.1 а.е. : %0.0f пк", scale/10);
			}
			else{}
			
			String^ str_m = gcnew String(str);
			label4->Text = str_m;
		}

		void ShowStars()
		{
			R1 = FindRadius(M1, Temp1);
			R2 = FindRadius(M2, Temp2);
			X_center = double(pictureBox1->Width) / 2;
			Y_center = double(pictureBox1->Height) / 2;

			X1 = X_center - r_star * scale * M2/(M1+M2);
			X2 = X1 + r_star * scale;
			Y1 = Y_center;
			Y2 = Y_center;

			double solscale = scale * sol;
			RectangleF Rect1 = RectangleF(X1 - R1 * solscale / 2, Y1 - R1 * solscale / 2, R1 * solscale, R1 * solscale);
			RectangleF Rect2 = RectangleF(X2 - R2 * solscale / 2, Y2 - R2 * solscale / 2, R2 * solscale, R2 * solscale);
			
			myPen = gcnew Pen(StarColour(Temp1), R1 * solscale);
			myBuffer->Graphics->DrawEllipse(myPen, Rect1);

			myPen = gcnew Pen(StarColour(Temp2), R2 * solscale);
			myBuffer->Graphics->DrawEllipse(myPen, Rect2);
		}

		void ShowOrbit()
		{
			myPen = gcnew Pen(System::Drawing::Color::FromArgb(255, 0, 0, 0), 2);
			char str[128];
			if (v == -1)
			{
				v = pow(6.6743e-11 * 2e30 * (M1 + M2) / r_planet / au, 0.5) / 1000;

				sprintf(str, "Скорость планеты: %0.2f км/c", v);
				String^ str_m = gcnew String(str);
				label10->Text = str_m;
				
				OrbitMain(scale, n, dt, X_center, Y_center, X1, X2, Y1, Y2, M1, M2, r_planet, v *1000 * scale / au);
				v = -1;
			}
			else
			{
				sprintf(str, "Скорость планеты: %0.2f км/c", v);
				String^ str_m = gcnew String(str);
				label10->Text = str_m;

				OrbitMain(scale, n, dt, X_center, Y_center, X1, X2, Y1, Y2, M1, M2, r_planet, v*1000 * scale / au);
			}

			ifstream in("orbit.txt");
			double x, y, T;
			char TT[32];
			in >> T >> TT;

			System::Drawing::Drawing2D::GraphicsPath^ path = gcnew System::Drawing::Drawing2D::GraphicsPath();

			while (in >> x >> y)
			{
				if ((x > (pictureBox1->Width)) || (y > pictureBox1->Height) || (x < 0) || (y < 0)) {}
				else
				{
					PointF p1 = PointF(x, y);
					PointF p2 = PointF(x + 1, y + 1);
					path->AddLine(p1, p2);
					path->CloseFigure();
				}
			}
			myBuffer->Graphics->DrawPath(myPen, path);
			in.close();

			sprintf(str, "Период: t = %0.2fT (%0.2f %s)", n, T, TT);
			String^ str_m = gcnew String(str);
			label8->Text = str_m;
		}
		void GrphClear()
		{
			myBuffer->Graphics->Clear(pictureBox1->BackColor);
			myBuffer->Render();
		}

		void ObjIniz()
		{
			char str[128];

			G = Gf(scale);

			ShowScale();
			ShowStars();
			ShowOrbit();

			sprintf(str, "Массы звёзд: %0.2f и %0.2f", M1, M2);
			String^ str_m = gcnew String(str);
			label1->Text = str_m;

			sprintf(str, "Расстояние между\nзвёздами: %0.2f а.е.", r_star);
			str_m = gcnew String(str);
			label5->Text = str_m;

			sprintf(str, "Радиусы звёзд:\nR1 = %0.2f Sol \nR2 = %0.2f Sol", R1, R2);
			str_m = gcnew String(str);
			label6->Text = str_m;

			sprintf(str, "Большая полуось орбиты планеты: r = %0.2f a.e.", r_planet);
			str_m = gcnew String(str);
			label7->Text = str_m;

			sprintf(str, "Интервал dt = %0.3f сут", dt);
			str_m = gcnew String(str);
			label9->Text = str_m;

			sprintf(str, "Температуры: %0.0f и %0.0f К", Temp1, Temp2);
			str_m = gcnew String(str);
			label14->Text = str_m;

			//sprintf(str, "G = %0.5f\nau = %0.10f\nsol = %0.5f", G,au, sol);
			//str_m = gcnew String(str);
			//label21->Text = str_m;

			myBuffer->Render();
		}

		Color StarColour(double T)
		{
			Color col;
			if ((T >= 2000) && (T < 3500)) { col = Color(System::Drawing::Color::FromArgb(255, 255, 96, 96)); };      // M
			if ((T >= 3500) && (T < 5000)) { col = Color(System::Drawing::Color::FromArgb(255, 255, 196, 111)); };    // K
			if ((T >= 5000) && (T < 6000)) { col = Color(System::Drawing::Color::FromArgb(255, 255, 242, 161)); };    // G
			if ((T >= 6000) && (T < 7500)) { col = Color(System::Drawing::Color::FromArgb(255, 255, 249, 197)); };    // F
			if ((T >= 7500) && (T < 11000)) { col = Color(System::Drawing::Color::FromArgb(255, 255, 255, 255)); };   // A
			if ((T >= 11000) && (T < 25000)) { col = Color(System::Drawing::Color::FromArgb(255, 202, 215, 255)); };  // B
			if (T >= 50000) { col = Color(System::Drawing::Color::FromArgb(255, 154, 175, 255)); };                   // O
			return col;
		}

		double FindRadius(double M, double T)
		{
			double L;
			if		(M <= 0.43)						{ L = 0.23 * pow(M, 2.3); }
			if		((0.43 < M) && (M <= 2.0))		{ L = pow(M, 4); }
			if		((2.0 < M) && (M <= 20.0))		{ L = 1.5 * pow(M, 3.5); }
			if		(20.0 < M)						{ L = 3200 * M; }

			return pow(L*pow(SolTemp/T,4), 0.5);
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ textBox3;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ textBox4;
	private: System::Windows::Forms::TrackBar^ trackBar2;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::TrackBar^ trackBar3;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::TextBox^ textBox5;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::TextBox^ textBox6;
	private: System::Windows::Forms::TextBox^ textBox7;
	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::Label^ label16;
	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::Label^ label18;
	private: System::Windows::Forms::Label^ label19;
	private: System::Windows::Forms::Label^ label20;
	private: System::Windows::Forms::Label^ label21;


	protected:


	private:
		/// <summary>
		/// Требуется переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->trackBar3 = (gcnew System::Windows::Forms::TrackBar());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::DarkSlateBlue;
			this->pictureBox1->Location = System::Drawing::Point(5, 5);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(990, 990);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox1_Paint);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(1095, 57);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 26);
			this->textBox1->TabIndex = 3;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(1095, 86);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 26);
			this->textBox2->TabIndex = 5;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(1058, 34);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(107, 20);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Массы звёзд";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(1058, 60);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(31, 20);
			this->label2->TabIndex = 2;
			this->label2->Text = L"M1";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(1058, 89);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(31, 20);
			this->label3->TabIndex = 4;
			this->label3->Text = L"M2";
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(1062, 549);
			this->trackBar1->Maximum = 5000;
			this->trackBar1->Minimum = 10;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(253, 69);
			this->trackBar1->SmallChange = 10;
			this->trackBar1->TabIndex = 12;
			this->trackBar1->Value = 2000;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &Form1::trackBar1_Scroll);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(1058, 523);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(79, 20);
			this->label4->TabIndex = 11;
			this->label4->Text = L"Масштаб";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(1333, 136);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(151, 40);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Расстояние между\nзвёздами";
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(1337, 179);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(100, 26);
			this->textBox3->TabIndex = 7;
			this->textBox3->TextChanged += gcnew System::EventHandler(this, &Form1::textBox3_TextChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(1333, 34);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(123, 20);
			this->label6->TabIndex = 8;
			this->label6->Text = L"Радиусы звёзд";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(1333, 267);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(278, 20);
			this->label7->TabIndex = 9;
			this->label7->Text = L"Большая полуось орбиты планеты:";
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(1337, 290);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(100, 26);
			this->textBox4->TabIndex = 10;
			this->textBox4->TextChanged += gcnew System::EventHandler(this, &Form1::textBox4_TextChanged);
			// 
			// trackBar2
			// 
			this->trackBar2->Location = System::Drawing::Point(1062, 473);
			this->trackBar2->Maximum = 5000;
			this->trackBar2->Minimum = 1;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(253, 69);
			this->trackBar2->SmallChange = 10;
			this->trackBar2->TabIndex = 15;
			this->trackBar2->Value = 1000;
			this->trackBar2->Scroll += gcnew System::EventHandler(this, &Form1::trackBar2_Scroll);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(1058, 448);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(139, 20);
			this->label8->TabIndex = 16;
			this->label8->Text = L"Кол-во периодов";
			// 
			// trackBar3
			// 
			this->trackBar3->Location = System::Drawing::Point(1062, 398);
			this->trackBar3->Maximum = 5000;
			this->trackBar3->Minimum = 1;
			this->trackBar3->Name = L"trackBar3";
			this->trackBar3->Size = System::Drawing::Size(253, 69);
			this->trackBar3->TabIndex = 100;
			this->trackBar3->Value = 500;
			this->trackBar3->Scroll += gcnew System::EventHandler(this, &Form1::trackBar3_Scroll);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(1058, 375);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(102, 20);
			this->label9->TabIndex = 18;
			this->label9->Text = L"Интервал dt";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(1062, 267);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(151, 20);
			this->label10->TabIndex = 19;
			this->label10->Text = L"Скорость планеты";
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(1066, 288);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(100, 26);
			this->textBox5->TabIndex = 20;
			this->textBox5->TextChanged += gcnew System::EventHandler(this, &Form1::textBox5_TextChanged);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(1172, 291);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(40, 20);
			this->label11->TabIndex = 21;
			this->label11->Text = L"км/с";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(1202, 60);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(32, 20);
			this->label12->TabIndex = 22;
			this->label12->Text = L"Sol";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(1202, 89);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(32, 20);
			this->label13->TabIndex = 23;
			this->label13->Text = L"Sol";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(1062, 136);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(110, 20);
			this->label14->TabIndex = 24;
			this->label14->Text = L"Температуры";
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(1095, 162);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(100, 26);
			this->textBox6->TabIndex = 25;
			this->textBox6->TextChanged += gcnew System::EventHandler(this, &Form1::textBox6_TextChanged);
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(1095, 190);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(100, 26);
			this->textBox7->TabIndex = 26;
			this->textBox7->TextChanged += gcnew System::EventHandler(this, &Form1::textBox7_TextChanged);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(1062, 163);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(27, 20);
			this->label15->TabIndex = 27;
			this->label15->Text = L"T1";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(1062, 193);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(27, 20);
			this->label16->TabIndex = 28;
			this->label16->Text = L"T2";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(1202, 167);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(19, 20);
			this->label17->TabIndex = 29;
			this->label17->Text = L"K";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(1202, 195);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(19, 20);
			this->label18->TabIndex = 30;
			this->label18->Text = L"K";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(1444, 182);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(35, 20);
			this->label19->TabIndex = 31;
			this->label19->Text = L"a.e.";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(1443, 296);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(35, 20);
			this->label20->TabIndex = 32;
			this->label20->Text = L"а.е.";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(1617, 43);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(13, 20);
			this->label21->TabIndex = 33;
			this->label21->Text = L" ";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1837, 923);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->textBox7);
			this->Controls->Add(this->textBox6);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->trackBar2);
			this->Controls->Add(this->trackBar3);
			this->DoubleBuffered = true;
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Name = L"Form1";
			this->Text = L"Form";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	private: double scale;
	private: double n;
	private: double dt; // в днях

	private: double r_star;
	private: double r_planet;

	private: double x;
	private: double y;
	private: double v = 0;
	private: double v_base;

	private: double X1;
	private: double X2;
	private: double Y1;
	private: double Y2;

	private: double X_center;
	private: double Y_center;

	private: double M1;
	private: double M2;
	private: double L1;
	private: double L2;
	private: double R1;
	private: double R2;
	private: double Temp1;
	private: double Temp2;

	private: Pen^ myPen;

	private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e)
	{
		Graphics^ Gr01 = pictureBox1->CreateGraphics();
		BufferedGraphicsContext^ DC = BufferedGraphicsManager::Current;
		myBuffer = DC->Allocate(Gr01, System::Drawing::Rectangle::Rectangle(0, 0, pictureBox1->Size.Width, pictureBox1->Size.Height));
		GrphClear();
		BaseSettings();
		ObjIniz();
	}

	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();
		try {
			M1 = (double)Convert::ToDouble(textBox1->Text);
			if (M1 > 0)
			{
				ObjIniz();
			}
		}
		catch (...) {}
	}

	private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();
		try {
			M2 = (double)Convert::ToDouble(textBox2->Text);
			if(M2 > 0)
			{
				ObjIniz();
			}
		}
		catch (...) {}
	}

	private: System::Void textBox3_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();
		try {
			r_star = (double)Convert::ToDouble(textBox3->Text);
			ObjIniz();
		}
		catch (...) {}
	}

	private: System::Void textBox4_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();
		try {
			r_planet = (double)Convert::ToDouble(textBox4->Text);
			ObjIniz();
		}
		catch (...) {}
	}

	private: System::Void textBox5_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();

		if ((String^)Convert::ToString(textBox5->Text) == "first")
		{
			v = -1;
			ObjIniz();
		}
		else
		{
			try {
				v = (double)Convert::ToDouble(textBox5->Text);
				if ((v > 0) && (v < 3000))
				{
					v = (double)Convert::ToDouble(textBox5->Text);
					ObjIniz();
				}
			}
			catch (...) {}
		}
		
	}

	private: System::Void pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		myBuffer->Render();
	}
	private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		scale = (double)Convert::ToDouble(trackBar1->Value);
		GrphClear();
		ObjIniz();	
	}

	private: System::Void trackBar2_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();
		n = (double)Convert::ToDouble(trackBar2->Value)/100;
		ObjIniz();
	}

	private: System::Void trackBar3_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();
		dt = (double)Convert::ToDouble(trackBar3->Value) / 1000;
		ObjIniz();
	}
	private: System::Void textBox6_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();
		try {
			Temp1 = (double)Convert::ToDouble(textBox6->Text);
			if ((Temp1 >= 2500) && (Temp1 <= 50000))
			{
				Temp1 = (double)Convert::ToDouble(textBox6->Text);
				ObjIniz();
			}
		}
		catch (...) {}
	}
	private: System::Void textBox7_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GrphClear();
		try {
			Temp2 = (double)Convert::ToDouble(textBox7->Text);
			if ((Temp2 >= 2500) && (Temp2 <= 50000))
			{
				Temp2 = (double)Convert::ToDouble(textBox7->Text);
				ObjIniz();
			}
		}
		catch (...) {}
	}
};
}

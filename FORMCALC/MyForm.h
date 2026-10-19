#pragma once
#include <iostream>
#include <time.h>
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text;

namespace FORMCALC {

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{

			InitializeComponent();

			P1 = gcnew array<int, 2>(N + 2, N + 2);
			P2 = gcnew array<int, 2>(N + 2, N + 2);

			for (int i = 0; i < N + 2; i++)
				for (int j = 0; j < N + 2; j++)
				{
					P1[i, j] = 0;
					P2[i, j] = 0;
				}

			globalStep = 0;
			firstClick = true;
			message = "";
			messageBits = "";
			keyStream = "";
			totalSteps = 0;
			stepCount = 0;

		}
	private:
		System::Windows::Forms::Button^ button2;
		System::Windows::Forms::Button^ button1;
		System::Windows::Forms::TextBox^ textBox1;
		System::Windows::Forms::Label^ label1;
		System::Windows::Forms::TextBox^ textBox3;
		System::Windows::Forms::Label^ label2;
		System::Windows::Forms::Label^ label3;
		System::Windows::Forms::PictureBox^ pictureBox1;
		System::ComponentModel::Container^ components;
		System::Windows::Forms::TextBox^ textBox2;

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		static const int N = 40;
		array<int, 2>^ P1;
		array<int, 2>^ P2;

		int globalStep;
		bool firstClick;
		String^ message;
		String^ messageBits;
		String^ keyStream;
		int totalSteps;
		int stepCount;

		// Правила
		int Rule0(int state, int neighbors)
		{
			if (neighbors == 2)
			{
				return state;
			}
			else if (neighbors == 3)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		int Rule1(int state, int neighbors)
		{
			if (neighbors == 1 || neighbors == 2 || neighbors == 3 || neighbors == 4)
			{
				return 1;
			}
			else if (neighbors == 5)
			{
				return state;
			}

			else
			{
				return 0;
			}
		}

		// Количество живых клеток
		int CountLiveCells()
		{
			int count = 0;
			for (int i = 1; i <= N; i++)
				for (int j = 1; j <= N; j++)
					count += P1[i, j];
			return count;
		}

		// Инициализация поля ключом
		void InitFromKey(int key)
		{
			srand(key);
			for (int i = 1; i <= N; i++)
				for (int j = 1; j <= N; j++)
					P1[i, j] = rand() % 2;
		}

		// Получение бита ключа из центра
		int GetKeyBit()
		{
			int center = N / 2;
			int bit = P1[center, center];
			Console::WriteLine(L"Бит: " + bit);
			return bit;
		}

		// Вычисление следующего шага
		void Calc()
		{
			int sum;
			int liveCells = CountLiveCells();
			Console::WriteLine(L"Живых клеток: " + liveCells);

			for (int i = 1; i <= N; i++)
			{
				for (int j = 1; j <= N; j++)
				{
					sum = 0;
					for (int ii = i - 1; ii <= i + 1; ii++)
					{
						for (int jj = j - 1; jj <= j + 1; jj++)
						{
							sum += P1[ii, jj];
						}
					}
					sum -= P1[i, j];

					if (liveCells > 300)
					{
						P2[i, j] = Rule0(P1[i, j], sum);
					}
					else
					{
						P2[i, j] = Rule1(P1[i, j], sum);
					}
				}
			}

			globalStep++;
		}

		// Копирование
		void Copy()
		{
			for (int i = 1; i <= N; i++)
				for (int j = 1; j <= N; j++)
					P1[i, j] = P2[i, j];
		}

		// Преобразование символа в биты
		String^ CharToBits(char c)
		{
			String^ bits = "";
			unsigned char uc = (unsigned char)c;

			for (int i = 7; i >= 0; i--)
			{
				unsigned char bit = (uc >> i) & 1;

				if (bit == 0)
					bits += "0";
				else
					bits += "1";
			}
			return bits;
		}

		// Преобразование битов в символ
		char BitsToChar(String^ bits)
		{
			if (bits->Length < 8)
				return 0;

			unsigned char c = 0;

			for (int i = 0; i < 8; i++)
			{
				c = c << 1;

				if (bits[i] == '1')
					c = c | 1;
			}

			// Возвращаем как char
			return (char)c;
		}

		// Преобразование текста в биты
		String^ TextToBits(String^ text)
		{
			String^ allBits = "";
			for (int i = 0; i < text->Length; i++)
			{
				allBits += CharToBits(text[i]);
			}
			return allBits;
		}

		// Преобразование битов в текст
		String^ BitsToText(String^ bits)
		{
			String^ text = "";
			for (int i = 0; i < bits->Length; i += 8)
			{
				String^ eightBits = "";
				for (int j = 0; j < 8 && (i + j) < bits->Length; j++)
				{
					eightBits += bits[i + j];
				}

				while (eightBits->Length < 8)
				{
					eightBits += "0";
				}

				char c = BitsToChar(eightBits);
				text += gcnew String(&c, 0, 1);
			}
			return text;
		}

		// XOR для двух символов
		String^ XOR(char dataBit, char keyBit)
		{
			if (dataBit == keyBit)
				return "0";
			else
				return "1";
		}

		// XOR для двух строк битов
		String^ XORStrings(String^ data, String^ key)
		{
			String^ result = "";
			int len = data->Length;
			for (int i = 0; i < len; i++)
			{
				result += XOR(data[i], key[i]);
			}
			return result;
		}

		// Генерация ключевого потока
		String^ GenerateKeyStream(int length)
		{
			String^ stream = "";
			globalStep = 0;

			for (int step = 0; step < length; step++)
			{
				Calc();

				int bit = GetKeyBit();

				if (bit == 0)
					stream += "0";
				else
					stream += "1";

				Copy();
			}

			return stream;
		}

		// Шифрование
		String^ Encrypt(String^ message, int key)
		{
			InitFromKey(key);
			String^ messageBits = TextToBits(message);
			String^ keyBits = GenerateKeyStream(messageBits->Length);
			String^ encryptedBits = XORStrings(messageBits, keyBits);

			return BitsToText(encryptedBits);
		}

		// Дешифровка
		String^ Decrypt(String^ cipherText, int key)
		{
			return Encrypt(cipherText, key);
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(580, 148);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(167, 72);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Шаг";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(566, 98);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(234, 22);
			this->textBox1->TabIndex = 4;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(491, 60);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(47, 17);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Ключ:";
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(566, 60);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(234, 22);
			this->textBox3->TabIndex = 7;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(488, 101);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(50, 17);
			this->label2->TabIndex = 8;
			this->label2->Text = L"Текст:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(488, 334);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(80, 17);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Результат:";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(12, 60);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(470, 470);
			this->pictureBox1->TabIndex = 11;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::pictureBox1_Paint);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(580, 238);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(167, 72);
			this->button2->TabIndex = 12;
			this->button2->Text = L"Результат";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(580, 331);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->ReadOnly = true;
			this->textBox2->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox2->Size = System::Drawing::Size(209, 135);
			this->textBox2->TabIndex = 13;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(812, 599);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"Шифратор";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		if (firstClick) {
			int key = Convert::ToInt32(textBox3->Text);
			message = textBox1->Text;

			InitFromKey(key);
			messageBits = TextToBits(message);

			keyStream = "";
			globalStep = 0;

			totalSteps = messageBits->Length;

			button1->Text = "Шаг " + stepCount + "/" + totalSteps;

			firstClick = false;
		}
		else {
			Calc();
			pictureBox1->Invalidate();

			int bit = GetKeyBit();
			keyStream += (bit == 0 ? "0" : "1");

			Copy();

			stepCount++;
			button1->Text = "Шаг " + stepCount + "/" + totalSteps;

			if (stepCount == totalSteps) {
				String^ encryptedBits = XORStrings(messageBits, keyStream);
				String^ encryptedText = BitsToText(encryptedBits);

				textBox2->Text = encryptedText;

				firstClick = true;
				button1->Text = "Шифровать";
				stepCount = 0;

				MessageBox::Show("Шифрование завершено!");
			}
		}
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {

		int key = Convert::ToInt32(textBox3->Text);
		String^ message = textBox1->Text;

		String^ encrypted = Encrypt(message, key);
		textBox2->Text = encrypted;
		pictureBox1->Invalidate();

		firstClick = true;
		button1->Text = "Шифровать";
		stepCount = 0;
		globalStep = 0;
		keyStream = "";
		messageBits = "";
	}
	private: System::Void pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		Graphics^ g = e->Graphics;
		int cellSize = pictureBox1->Width / N;
		if (cellSize < 1)
			cellSize = 1;

		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= N; j++)
			{
				int x = (j - 1) * cellSize;
				int y = (i - 1) * cellSize;

				if (P1[i, j] == 1)
					g->FillRectangle(Brushes::Purple, x, y, cellSize - 1, cellSize - 1);
				else
					g->FillRectangle(Brushes::White, x, y, cellSize - 1, cellSize - 1);

				g->DrawRectangle(Pens::LightGray, x, y, cellSize - 1, cellSize - 1);
			}
		}

		int center = N / 2;
		int centerX = (center)*cellSize;
		int centerY = (center)*cellSize;
		g->DrawRectangle(Pens::Red, centerX, centerY, cellSize - 1, cellSize - 1);
	}
	};
}
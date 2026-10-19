#include <iostream>
#include <windows.h>
#include <time.h>
#include <string>

using namespace std;

const int N = 40;

int P1[N + 2][N + 2] = { 0 };
int P2[N + 2][N + 2] = { 0 };

string keyStream = "";
int keyBitIndex = 0;

int Rule0(int state, int neighbors)
{
	if (neighbors == 2 or neighbors == 3)
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
	if (neighbors == 2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Rule2(int state, int neighbors)
{
	if (neighbors == 3 or neighbors == 6 or neighbors == 7 or neighbors == 8)
	{
		return 1;
	}
	else if (neighbors == 4 or neighbors == 3 or neighbors == 6 or neighbors == 7 or neighbors == 8)
	{
		return state;
	}
	else
	{
		return 0;
	}
}

int Rule3(int state, int neighbors)
{
	if (neighbors == 2 or neighbors == 4 or neighbors == 5)
	{
		return state;
	}
	else if (neighbors == 3 or neighbors == 6)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int globalStep = 0;

void InitFromKey(int key)
{
	srand(key);
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			P1[i][j] = rand() % 2;
}

int GetKeyBit()
{
	int center = N / 2;
	return P1[center][center];
}

void Calc()
{
	int sum;
	int ruleNumber = globalStep % 4;

	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			sum = 0;
			for (int ii = i - 1; ii <= i + 1; ii++)
			{
				for (int jj = j - 1; jj <= j + 1; jj++)
				{
					sum += P1[ii][jj];
				}
			}
			sum -= P1[i][j];

			if (ruleNumber == 0)
			{
				P2[i][j] = Rule0(P1[i][j], sum);
			}
			else if (ruleNumber == 1)
			{
				P2[i][j] = Rule1(P1[i][j], sum);
			}
			else if (ruleNumber == 2)
			{
				P2[i][j] = Rule2(P1[i][j], sum);
			}
			else
			{
				P2[i][j] = Rule3(P1[i][j], sum);
			}
		}
	}

	globalStep++;
}

void Copy()
{
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			P1[i][j] = P2[i][j];
}

char XOR(char dataBit, char keyBit)
{
	int dataNumber, keyNumber;

	if (dataBit == '0')
		dataNumber = 0;
	else
		dataNumber = 1;

	if (keyBit == '0')
		keyNumber = 0;
	else
		keyNumber = 1;

	int result = dataNumber ^ keyNumber;

	if (result == 0)
		return '0';
	else
		return '1';
}

void GenerateKeyStream(int length)
{
	keyStream = "";
	keyBitIndex = 0;
	globalStep = 0;

	for (int step = 0; step < length; step++)
	{
		Calc();

		int bit = GetKeyBit();

		if (bit == 0)
			keyStream += '0';
		else
			keyStream += '1';

		Copy();
	}
}

string CharToBits(char c)
{
	string bits = "";
	for (int i = 7; i >= 0; i--)
	{
		int shifted = c >> i;
		int bit = shifted & 1;

		if (bit == 0)
			bits += '0';
		else
			bits += '1';
	}

	return bits;
}

char BitsToChar(string bits)
{
	if (bits.length() < 8)
		return 0;

	char c = 0;

	for (int i = 0; i < 8; i++)
	{
		c = c << 1;

		if (bits[i] == '1')
			c = c | 1;
	}

	return c;
}

string Encrypt(string message, int key)
{
	InitFromKey(key);

	string allBits = "";
	for (int i = 0; i < message.length(); i++)
	{
		allBits += CharToBits(message[i]);
	}

	GenerateKeyStream(allBits.length());

	string encryptedBits = "";
	for (int i = 0; i < allBits.length(); i++)
	{
		char resultBit = XOR(allBits[i], keyStream[i]);
		encryptedBits += resultBit;
	}

	string encryptedText = "";
	for (int i = 0; i < encryptedBits.length(); i += 8)
	{
		string eightBits = "";
		for (int j = 0; j < 8 && (i + j) < encryptedBits.length(); j++)
		{
			eightBits += encryptedBits[i + j];
		}

		while (eightBits.length() < 8)
		{
			eightBits += '0';
		}

		char c = BitsToChar(eightBits);
		encryptedText += c;
	}

	return encryptedText;
}

string Decrypt(string cipherText, int key)
{
	return Encrypt(cipherText, key);
}

int main()
{
	setlocale(LC_ALL, "Russian");

	// Тест 1
	string testMessage = "ABC";
	int testKey = 4;

	cout << "Тест 1:" << endl;
	cout << "Ключ: " << testKey << endl;
	cout << "Сообщение: " << testMessage << endl;

	string encrypted = Encrypt(testMessage, testKey);
	cout << "Зашифровано: ";
	for (int i = 0; i < encrypted.length(); i++)
	{
		cout << (int)(unsigned char)encrypted[i] << " ";
	}
	cout << endl;

	string decrypted = Decrypt(encrypted, testKey);
	cout << "Расшифровано: " << decrypted << endl;

	if (testMessage == decrypted)
		cout << "Получилось!" << endl;
	else
		cout << "Ошибка!" << endl;

	int userKey;
	string userMessage;

	cout << endl << "Введите ключ (любое число): ";
	cin >> userKey;
	cin.ignore();
	cout << "Введите сообщение (английскими буквами): ";
	getline(cin, userMessage);

	cout << "Результат" << endl;
	cout << "Сообщение: " << userMessage << endl;

	string userEncrypted = Encrypt(userMessage, userKey);
	cout << "Зашифровано: ";
	for (int i = 0; i < userEncrypted.length(); i++)
	{
		cout << (int)(unsigned char)userEncrypted[i] << " ";
	}
	cout << endl;
	cout << "Зашифровано: " << userEncrypted << endl;

	string userDecrypted = Decrypt(userEncrypted, userKey);
	cout << "Расшифровано: " << userDecrypted << endl;

	cin.get();
}
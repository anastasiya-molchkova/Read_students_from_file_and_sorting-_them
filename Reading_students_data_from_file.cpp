/* Напишите программу учёта оценок студентов. Для этого создайте текстовый файл с именем input_data.txt, 
содержащий список из 10 студентов и их оценки по трём предметам: математике, физике, информатике. 
Содержимое файла:
   в первой строке находится общее количество студентов;
   в каждой последующей строке находится ФИО студента и три целых числа (оценки);
   данные в строке разделены пробелами, а оценки варьируются в диапазоне от 1 до 5.
Затем создайте класс, с помощью которого вы будете считывать данные с файла. 
На экран выведите ФИО студентов с оценками в порядке убывания их среднего бала. */

#include <iostream>
#include <string>  // для работы со строками
#include <fstream> // для работы с файлами
#include <vector>
#include <sstream> // для работы со строковым потоком, чтобы превращать данные из строки в нужные данные разного типа
#include <algorithm> // для использования алгоритмов сортировки

using std::cout; using std::string; using std::vector;

struct Full_name
{
	string surname_;
	string name_;
	string patronymic_;
	Full_name() {}
	Full_name(string surname, string name, string patronymic): 
		surname_(surname), name_(name), patronymic_(patronymic) 
		{}
};

// класс данных о студенте - ФИО и оценки
class Student_data
{
public:
	enum marks
	{one = 1, bad, satisfactorily, good, excellent};

	Full_name full_name_;
	marks math_;
	marks physics_;
	marks info_technology_;
	double average_score_;

	Student_data() {}
	Student_data(string surname, string name, string patronymic, marks first_mark, marks second_mark, marks third_mark):
		full_name_(Full_name(surname, name, patronymic)), math_(first_mark), physics_(second_mark), info_technology_(third_mark),
		average_score_(static_cast<double>(first_mark+second_mark+third_mark)/3)
	{}

	// перегрузка оператора вывода данных студента
	friend std::ostream& operator<<(std::ostream& out, const Student_data& student)
	{
		out << student.full_name_.surname_ << " " << student.full_name_.name_ << " " << student.full_name_.patronymic_ 
			<< " " << student.math_ << " " << student.physics_ << " " << student.info_technology_ 
			// << " " << student.average_score_
			<<'\n';
		return out;
	}
};

// класс-контейнер для обработки данных о студентах
class The_list_of_students
{
private:
	vector<Student_data*> students; // вектор из указателей на класс
	unsigned number_of_students;
public:
	The_list_of_students(): students{}, number_of_students()
	{}
	void set_number(const int some_number)
	{
		if (some_number < 0) cout << "Не удалось задать количество студентов" << '\n';
		else number_of_students = some_number;
	}

	// добавление данных о ещё одном студенте через указатель
	void add_student(Student_data* student)
	{
		students.push_back(student);
		if (number_of_students < students.size()) number_of_students = students.size();
	}

	// сортировка элементов вектора по среднему баллу от большего к меньшему
	void sort_by_average()
	{
		// Перебираем каждый элемент массива (кроме последнего, он уже будет отсортирован к тому времени, когда мы до него доберёмся)
		for (unsigned startIndex = 0; startIndex < (number_of_students - 1); ++startIndex)
		{
			// В переменной maximumIndex хранится индекс наибольшего значения, которое мы нашли в этой итерации
			// Начинаем с того, что наибольший элемент в этой итерации - это первый элемент (индекс 0)
			int maximumIndex = startIndex;

			// Затем ищем элемент поменьше в остальной части массива
			for (unsigned currentIndex = startIndex + 1; currentIndex < number_of_students; ++currentIndex)
			{
				// Если мы нашли элемент, который больше нашего наибольшего элемента,
				if (students.at(currentIndex)->average_score_ > students.at(maximumIndex)->average_score_)
					// то запоминаем его
					maximumIndex = currentIndex;
			}
			// maximumIndex теперь наибольший элемент 

			// Меняем местами наше начальное наибольшее число с тем, которое мы обнаружили
			std::swap(students[startIndex], students[maximumIndex]);
		}
	}

	// печать всех элементов массива
	void print()
	{
		cout << '\n';
		if (students.size() == 0)
			cout << "Список студентов пуст!" << '\n';
		else for (unsigned short i=0; i< number_of_students; i++)
			cout << *students[i];
	}

	// деструктор класса
	~The_list_of_students()
	{
		if (students.size() > 0)
			for (unsigned short i = 0; i < number_of_students; i++)
				delete students[i]; // освобождаем память каждого указателя
	}
};

// объявление вспомогательных функций для чтения данных из файла:
bool line_is_ok(const string& line_from_file);
Student_data* convert_string_to_pointer(string correct_string);
unsigned short get_number_of_students_from_file(const string& number_from_file);

// заполняем передаваемый массив данных о студентах данными из файла
void fill_from_file(The_list_of_students& array)
{	
	std::ifstream data_file("input_data.txt");
	if (!data_file)
	{
		std::cerr << "Файл невозможно прочитать!" << '\n';
		exit(1);
	}

	// считываем количество строк в файле
	string number_from_file;
	getline(data_file, number_from_file);
	unsigned short n_students = get_number_of_students_from_file(number_from_file);
	array.set_number(n_students);

	unsigned short lines_index = 0;
	while (lines_index < n_students)  // читаем только обозначенное в файле количество строк
	{
		string line_from_file;
		getline(data_file, line_from_file);
		if (line_is_ok(line_from_file))
		{
			auto student_ptr = convert_string_to_pointer(line_from_file);
			array.add_student(student_ptr);
		}
		else cout << "В файле есть некорректная строка" << '\n';
		++lines_index;
	}
}

int main()
{
	setlocale(LC_CTYPE, "rus");   // для вывода сообщений на кириллице

	The_list_of_students Students{};
	fill_from_file(Students);
	Students.sort_by_average();
	cout << "ФИО студентов с оценками в порядке убывания их среднего балла:";
	Students.print();

	return 0;
}

// ОПИСАНИЕ ВСПОМОГАТЕЛЬНЫХ ФУНКЦИЙ ДЛЯ ЧТЕНИЯ ИЗ ФАЙЛА:

// функция возвращает истину, если строка соответствует ожидаемому формату "Ф И О ц1 ц1 ц3" и ложь, если не соответствует
bool line_is_ok(const string& line_from_file)
{
	if (line_from_file.length() < 10) return false; //т.к. строка должна иметь вид "Ф И О ц1 ц1 ц3" - как минимум 11 символов
	unsigned short spaces_counter = 0;
	unsigned short index = 0;          // индекс символов строки

	while ((index < line_from_file.size()) && (spaces_counter < 6))
		// 1ый пробел - окончание фамилии, 2ой- окончание имени, 3ий - отчества, потом через пробел идут три оценки
	{
		if (isspace(line_from_file[index]))
			spaces_counter++;
		else
		{
			if ((spaces_counter < 3) && (!isalpha(line_from_file[index]))) return false;
			if ((spaces_counter >= 3) && (!isdigit(line_from_file[index]))) return false;
		}
		index++;
	}
	if (index < line_from_file.size()) return false; // если было 6 и более пробелов до конца строки
	else return true;
}

// функция превращает строку в правильном формате в указатель на класс с данными о студенте
Student_data* convert_string_to_pointer(string correct_string)
{
	std::stringstream string_from_file;
	string_from_file << correct_string;

	string surname, name, patronymic;
	string_from_file >> surname;
	string_from_file >> name;
	string_from_file >> patronymic;

	unsigned short math(0), physics(0), it(0);
	string_from_file >> math;
	string_from_file >> physics;
	string_from_file >> it;

	return new Student_data(surname, name, patronymic, static_cast<Student_data::marks>(math), static_cast<Student_data::marks>(physics), static_cast<Student_data::marks>(it));
}

// получаем количество строк для чтения из файла
unsigned short get_number_of_students_from_file(const string& number_from_file)
{
	std::stringstream number;
	number << number_from_file;
	unsigned short n(0);
	number >> n;
	return n;
}
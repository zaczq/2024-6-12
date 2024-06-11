//类和子函数
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <queue>
#include <stack>
#include <iomanip>
using namespace std;

struct CNode
{/*客户*/
	string name; /*姓名*/
	int number; /*订票量*/
	int grade; /*座位等级*/
	CNode* next;
	CNode(string n, int num, int g) : name(n), number(num), grade(g), next(nullptr) {}
};

struct HCNode
{/*候补客户*/
	string name;
	int number;
	int grade;
	HCNode(string n, int num, int g) : name(n), number(num), grade(g) {}
};

class airline
{/*航班信息*/
public:
	string head_station; /*起点站名*/
	string end_station; /*终点站名*/
	string air_num; /*航班号*/
	string air_kind; /*机型*/
	string day; /*飞行日期 星期几*/

	double price1; /*经济舱票价*/
	double price2; /*公务舱票价*/
	double price3; /*头等舱票价*/
	int people_quotal1; /*经济舱乘员定额*/
	int people_quotal2; /*公务舱乘员定额*/
	int people_quotal3; /*头等舱成员定额*/
	int ticket1; /*经济舱余票量*/
	int ticket2; /*公务舱余票量*/
	int ticket3; /*头等舱余票量*/

	CNode* peo1; /*经济舱订票客户*/
	CNode* peo2; /*公务舱订票客户*/
	CNode* peo3; /*头等舱订票客户*/
	queue<HCNode>que1; /*经济舱候补客户*/
	queue<HCNode>que2; /*公务舱候补客户*/
	queue<HCNode>que3; /*头等舱候补客户*/

	//构造函数(初始化成员变量)
	airline() : peo3(nullptr), peo1(nullptr), peo2(nullptr), head_station(""), end_station(""), air_num(""), air_kind(""), day(""),
		price1(1000), price3(4000), price2(2000), people_quotal1(80), people_quotal2(80), people_quotal3(40), ticket1(80), ticket2(80),
		ticket3(40) {}

	//析构函数(清空堆区内存)
	~airline()
	{
		clearList(peo1);
		clearList(peo2);
	}
	//订票
	bool orderTicket(const string& name, const int& num, const int& grade)
	{
		if (grade == 1 && ticket1 >= num) {
			addCustomer(peo1, name, num, grade);
			ticket1 -= num;
			return true;
		}
		else if (grade == 2 && ticket2 >= num) {
			addCustomer(peo2, name, num, grade);
			ticket2 -= num;
			return true;
		}
		else if (grade == 3 && ticket3 >= num) {
			addCustomer(peo3, name, num, grade);
			ticket3 -= num;
			return true;
		}
		return false; // 余票不足
	}
	//候补
	void wait(const string& name, const int& num, const int& grade)
	{
		HCNode temp(name, num, grade);
		if (grade == 1)
			que1.push(temp);
		else if (grade == 2)
			que2.push(temp);
		else
			que3.push(temp);
	}
	//退票
	bool cancelTicket(const string& name, const int& grade)
	{
		bool deleted = false;
		if (grade == 1) {
			deleted = deleteCustomer(peo1, name, ticket1);
			if (deleted) checkWaitList(que1, peo1, ticket1, grade);
		}
		else if (grade == 2) {
			deleted = deleteCustomer(peo2, name, ticket2);
			if (deleted) checkWaitList(que2, peo2, ticket2, grade);
		}
		else {
			deleted = deleteCustomer(peo3, name, ticket3);
			if (deleted) checkWaitList(que3, peo3, ticket3, grade);
		}
		return deleted;
	}
	//打印乘客信息
	void print_passenger(void)
	{
		cout << "----------   经济舱   ----------" << endl;
		print(peo1);
		cout << endl;
		cout << "----------   公务舱   ----------" << endl;
		print(peo2);
		cout << endl;
		cout << "----------   头等舱   ----------" << endl;
		print(peo3);
		cout << endl;
	}
private:
	//打印客户信息
	void print(CNode*& peo)
	{
		CNode* current = peo;
		while (current != nullptr)
		{
			cout << current->name << "  " << current->grade << "  " << current->number << endl;
			current = current->next;
		}
	}
	//检查候补队列，并为满足条件的客户订票 ---------------------------------- 并未满足题目要求，感觉题目不是特别恰当
	void checkWaitList(queue<HCNode>& que, CNode*& peo, int& ticket, int grade)
	{
		while (!que.empty() && ticket > 0)
		{
			const HCNode& temp = que.front();
			if (temp.number <= ticket)
			{
				addCustomer(peo, temp.name, temp.number, grade);
				ticket -= temp.number;
				que.pop(); // 从候补队列中移除已订票的客户
			}
			else
			{
				break; // 余票不足，无法满足当前候补客户
			}
		}
	}
	// 添加客户到链表
	void addCustomer(CNode*& head, string name, int num, int grade) 
	{
		CNode* temp = new CNode(name, num, grade);
		if (head == nullptr) 
		{
			head = temp;
		}
		else 
		{
			CNode* current = head;
			while (current->next != nullptr)
				current = current->next;
			current->next = temp;
		}
	}
	//删除客户出链表
	bool deleteCustomer(CNode*& head, string name, int& ticket)
	{
		if (head == nullptr)
			return false;
		//删除元素位于链表头部
		if (head->name == name)
		{
			CNode* temp = head;
			ticket += temp->number;
			head = head->next;
			delete temp;
			return true;
		}
		//不位于头部
		CNode* current = head;
		while (current->next != nullptr)
		{
			if (current->next->name == name) {
				CNode* temp = current->next;
				ticket += temp->number;
				current->next = temp->next;
				delete temp;
				return true;
			}
			current = current->next;
		}
		return false;
	}
	// 清理链表
	void clearList(CNode*& head) 
	{
		CNode* current = head;
		while (current != nullptr) {
			CNode* next = current->next;
			delete current;
			current = next;
		}
		head = nullptr;
	}
};
vector<airline>air;

istream& operator>>(istream& is, airline& p)/*重载>>运算符函数*/
{
	is >> p.head_station >> p.end_station >> p.air_num >> p.air_kind >> p.day >> p.price1
		>> p.price2 >> p.people_quotal1 >> p.people_quotal2 >> p.ticket1 >> p.ticket2;
	return is;
}

ostream& operator<<(ostream& is, airline& p)/*重载<<运算符函数*/
{
	is <<' '<< p.head_station <<' '<< p.end_station <<' '<< p.air_num <<' '<< p.air_kind
		<<' '<< p.day <<' '<< p.price1 <<' '<< p.price2 <<' '<< p.people_quotal1 <<' '<<
		p.people_quotal2 <<' '<< p.ticket1 <<' '<< p.ticket2;
	return is;
}

void print_table(int flag)/*打印表格*/
{
	if (flag == 1)//航班表头
	{
		cout << left;
		cout << setw(10) << "起点"
			<< setw(10) << "终点"
			<< setw(8) << "航班号"
			<< setw(6) << "机型"
			<< setw(5) << "时间"
			<< setw(12) << "经济舱票价"
			<< setw(12) << "公务舱票价"
			<< setw(12) << "头等舱票价"
			<< setw(12) << "经济舱余票"
			<< setw(12) << "公务舱余票"
			<< setw(12) << "头等舱余票"
			<< endl;
	}
	else//客户表头
	{

	}
}

void printQ(void)/*选择身份*/
{
	cout << "                 航空客运订票系统                " << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "---   1.用户         2.管理员         0.退出  ---" << endl;
	cout << "-------------------------------------------------" << endl;
}

void printD(int flag)/*打印登录页面*/
{
	if (flag == 1)//用户页面
	{
		cout << "                  用户登录页面                   " << endl;
		cout << "-------------------------------------------------" << endl;
		cout << "---   1.登录       2.注册       0.返回上一级  ---" << endl;
		cout << "-------------------------------------------------" << endl;
	}
	else//管理员页面
	{
		cout << "                 管理员登录页面                  " << endl;
		cout << "-------------------------------------------------" << endl;
		cout << "-------   1.登录            0.返回上一级  -------" << endl;
		cout << "-------------------------------------------------" << endl;
	}
}

void print_use(int flag)/*打印用户或管理员页面*/
{
	if (flag == 1)//用户
	{
		cout << "                   用户页面                      " << endl;
		cout << "-------------------------------------------------" << endl;
		cout << "---------------     1.查询航线     --------------" << endl;
		cout << "---------------     2.订票业务     --------------" << endl;
		cout << "---------------     3.退票业务     --------------" << endl;
		cout << "---------------     0.返回上一级   --------------" << endl;
		cout << "-------------------------------------------------" << endl;
	}
	else//管理员
	{
		cout << "                  管理员页面                     " << endl;
		cout << "-------------------------------------------------" << endl;
		cout << "-----------     1.打印所有航班信息     ----------" << endl;
		cout << "-----------     2.查看乘客信息         ----------" << endl;
		cout << "-----------     3.添加新的航班         ----------" << endl;
		cout << "-----------     0.返回上一级           ----------" << endl;
		cout << "-------------------------------------------------" << endl;
	}
}

bool login(string str)/*登录操作*/
{
	cout << "请输入您的账号：";
	string acc; cin >> acc;
	cout << "请输入您的密码：";
	string bcc; cin >> bcc;

	string result;//输入的账号密码
	result += acc; result += ' '; result += bcc;

	ifstream input;
	input.open(str);
	bool flag = false;
	if (input.is_open())
	{
		string line;
		while (getline(input, line))
		{
			if (result == line)
			{
				flag = true;
				break;
			}
		}
		input.close();
	}else
	{
		cout << "打开文件失败！";
		return false;
	}
	return flag;
}

bool enroll(void)/*注册操作*/
{
	cout << "请输入您的新账号：";
	string acc; cin >> acc;
	cout << "请输入您的新密码：";
	string bcc; cin >> bcc;

	string result;//输入的数据
	result += acc; result += ' '; result += bcc;
	ofstream output;
	output.open("D://user.tet", ios::app);
	if (output.is_open())
	{
		output << endl << result;
		output.close();
		return true;
	}
	else
	{
		return false;
	}
}
void Query(void)/*查询航班*/
{
	system("cls");
	cout << "                请选择您的查询方式               " << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "---  1.起始终点                     2.航班号  ---" << endl;
	cout << "---                0.返回上一级               ---" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "请输入您的选择：";
	char chose; cin >> chose;
	while (chose != '1' && chose != '2' && chose != '0')
	{
		cout << "对不起，无该选项，请您重新选择：";
		cin >> chose;
	}
	switch (chose)
	{
	case '1':
	{
		cout << "请输入您要查询航班的起点：";
		string q; cin >> q;//起点
		cout << "请输入您要查询航班的终点：";
		string z; cin >> z;//终点
		bool flag = true;//判断是否有该航班
		print_table(1);
		for (int i = 0; i < air.size(); i++)
		{
			if (air[i].head_station == q && air[i].end_station == z)
			{
				flag = false;
				cout << setw(10) << air[i].head_station << setw(10) << air[i].end_station << setw(8) << air[i].air_num
					<< setw(6) << air[i].air_kind << setw(5) << air[i].day << setw(12) << air[i].price1 << setw(12) <<
					air[i].price2 << setw(12) << air[i].price3 << setw(12) << air[i].ticket1 << setw(12) << air[i].ticket2
					<< setw(12) << air[i].ticket3 << endl;
			}
		}
		if (flag == true)
			cout << "对不起，未找到您要查询的航班！" << endl;
		break;
	}//case
	case '2':
	{
		cout << "请输入您要查询的航班号：";
		string id; cin >> id;
		bool flag = true;
		print_table(1);
		for (int i = 0; i < air.size(); i++)
		{
			if (air[i].air_num == id)
			{
				flag = false;
				cout << setw(10) << air[i].head_station << setw(10) << air[i].end_station << setw(8) << air[i].air_num
					<< setw(6) << air[i].air_kind << setw(5) << air[i].day << setw(12) << air[i].price1 << setw(12) <<
					air[i].price2 << setw(12) << air[i].price3 << setw(12) << air[i].ticket1 << setw(12) << air[i].ticket2
					<< setw(12) << air[i].ticket3 << endl;
			}
		}
		if (flag == true)
			cout << "对不起，未找到您要查询的航班！" << endl;
		break;
	}//case
	case '0':
		return;
	}//switch
	system("pause");
}

bool order(void) /*订票业务*/
{
	cout << "请输入您要订购的航班号：";
	string id; cin >> id;
	cout << "请输入您的出行日期：";
	string day; cin >> day;
	for (int i = 0; i < air.size(); i++)
	{
		if (air[i].air_num == id && air[i].day == day)
		{
			cout << "请输入您的姓名：";
			string name; cin >> name;
			cout << "请输入您要订购的舱位等级 (1.经济舱 2.公务舱 3.头等舱) ：";
			int grade; cin >> grade;
			while (grade != 1 && grade != 2 && grade != 3)
			{
				cout << "对不起，没有该选项，请您重新选择：";
				cin >> grade;
			}
			cout << "请输入您要订购的票数：";
			int num; cin >> num;
			if (air[i].orderTicket(name, num, grade))
				cout << "订购成功！" << endl;
			else
			{
				system("cls");
				cout << "--------------------------------------------" << endl;
				cout << "---对不起，余票不足，请问您是否需要候补？---" << endl;
				cout << "--------------------------------------------" << endl;
				cout << "---   1.Yes                       2.No   ---" << endl;
				cout << "--------------------------------------------" << endl;
				cout << "请输入您的选择：";
				int chose; cin >> chose;
				while (chose != 1 && chose != 2 )
				{
					cout << "对不起，无此选项，请您重新输入：";
					cin >> chose;
				}
				if (chose == 1)
				{
					air[i].wait(name, num, grade);
					cout << "候补成功！" << endl;
				}
				else
					break;
			}
			system("pause");
			return true;
		}//if
	}//for
	return false;
}

void return_tkt(void) /*退票业务*/
{
	cout << "请输入您要退订的航班号：";
	string num; cin >> num;
	cout << "请输入您要退订航班的日期 (星期几) ：";
	string day; cin >> day;
	for (int i = 0; i < air.size(); i++)
	{
		//找到要退订的航班
		if (air[i].air_num == num && air[i].day == day)
		{
			cout << "请输入您的姓名：";
			string name; cin >> name;
			cout << "请输入您的舱位等级（1.经济舱 2.公务舱 3.头等舱）：";
			int grade; cin >> grade;
			while (grade != 1 && grade != 2 && grade != 3)
			{
				cout << "对不起，没有该选项，请您重新选择：";
				cin >> grade;
			}
			if (air[i].cancelTicket(name, grade))
				cout << "退票成功！" << endl;
			else
				cout << "对不起，未找到您的客户信息！" << endl;
			system("pause");
			return;
		}
	}
	//未找到要退订的航班
	cout << "对不起，无此航班！" << endl;
	system("pause");
	return;
}

void print_all(void)/*打印所有航班*/
{
	for (int i = 0; i < air.size(); i++)
	{
		cout << air[i] << endl;
	}
}

void view_passenger(void)/*查看乘客信息*/
{
	cout << "请输入您要查询的航班号：";
	string id; cin >> id;
	cout << "请输入您要查询航班的日期：";
	string day; cin >> day;
	for (int i = 0; i < air.size(); i++)
	{
		if (id == air[i].air_num && day == air[i].day)
		{
			air[i].print_passenger();
			return;
		}
	}
	cout << "对不起，没有此航班信息！" << endl;
}

void add_air(void)/*添加航班信息*/
{
	cout << "请输入您要添加的航班号：";
	string id; cin >> id;
	cout << "请输入您要添加的机型：";
	string kind; cin >> kind;
	cout << "请输入航班的起点：";
	string q; cin >> q;
	cout << "请输入航班的终点：";
	string z; cin >> z;
	cout << "请您输入飞行日期：";
	string day; cin >> day;
	airline temp;
	temp.air_num = id; temp.air_kind = kind; temp.head_station = q; temp.end_station = z; temp.day = day;
	air.push_back(temp);
	cout << "添加航班成功！" << endl;
}
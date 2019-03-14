#include<iostream>
#include<string>
using namespace std;

const int max_len = 10;
const int max_level = 10;

class Target
{
    public:
    string title, words;
    int blg;//��������һlevel����һ��

    void print()//���
    {
        if(this->title.length())//���� title ����� title
        {
            cout << this->title << ' ';
        }
        if(this->words.length())//���� words ����� words
        {
            cout << this->words << ' ';
        }
        cout << this->blg << endl;//�����������һ level
    }
};

Target tar[max_len][max_level];

int t_in(string &a, string b, int b_start)//�����ַ����ú������ó����ַ����е�һ��ָ�ģ�û�����һ��ָ��
{
    int i = b_start;
    while(b[b_start] != ' ' && b[b_start] != ')')
    {   
        b_start ++;
    }
    a = b.substr(i, b_start - i);//c++֧���ַ������
    return b_start;//���ָ���λ���ǿո�򡮣���
}

int the_len_of_each_level[max_len] = {0};

int In_a_level(string text,int i,int level,int the_len_of_last_tar)//��the_len_of_last_tar����һ����level
{
    for(; ; )
    {
        if(text[i] == '(') level++;
        i = t_in(tar[the_len_of_each_level[level]][level].title, text, i + 1);
        cout<<"the title of ("<<the_len_of_each_level[level]<<","<<level<<") is "<<tar[the_len_of_each_level[level]][level].title<<endl;
        
        tar[the_len_of_each_level[level]][level].blg = the_len_of_last_tar;

        if(text[i + 1] != '(')
        {
            i = t_in(tar[the_len_of_each_level[level]][level].words, text, i + 1);
        cout<<"the words of ("<<the_len_of_each_level[level]<<","<<level<<") is "<<tar[the_len_of_each_level[level]][level].words<<endl;
        }else{
            i = In_a_level(text,i+1,level,the_len_of_each_level[level]);
        }
        the_len_of_each_level[level]++;
        if(text[i++] == ')')
        {
            level--;
        }
        if(text[i] == ' ') 
        {
        	i++;
        	continue;
		}
        else{
        	if(text[i] == ')')
        	{
        		level--;
        		break;
			}
		}
    }
    return i;
}

void In(string text)
{
    int level = -1;//��Ϊ���ַ����ĵ�0�ʼ�����Գ�ʼ��Ϊ -1 ����֤��һ��Ϊ 0
    	int i = 0; 
        if(text[i] == '(')
        {
            level++;
            i = t_in(tar[0][level].title, text, i + 1);
            tar[0][level].blg = -1;
            In_a_level(text,i+1,level,0);
        }
}


int main()
{
    string text;
    getline(cin, text);
    In(text);
    for(int i = 0; i < max_level; i ++)
    {
        for(int t = 0; t < max_len; t ++)
        {
            if(tar[t][i].title.length())
            {
                cout << "Level " << i << ' ';
                tar[t][i].print();
            }
        }
        cout << endl;
    }
    cout << text << endl;
}

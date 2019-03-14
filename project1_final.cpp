#include<iostream>
#include<string>
#include<fstream>
using namespace std;

const int max_len = 10;
const int max_level = 20;
const int length_of_letter = 6;
const int length_of_edge = 15;
const int distance_of_level = 70;
const int distance_horizon = 40;
const string color_fill_title = "#FFB6C1";
const string color_fill_words = "#A8A8A8";
const string color_words = "#000000";//words 文本的颜色
const string color_line = "#FF6EC7";//线颜色
const string font_family = "consolas";//字体选择
const double rect_height = 20;//矩形长度
const double line_vector = 4;//线的偏移量
const double move_px = 20;//偏移量
const int font_size = 12;//字体大小
const double title_pos[2] = {4, 15};//title 的相对定位
const double words_pos[2] = {4, 35};//words 的相对定位
double pic_width;//图片宽度
double pic_height;//图片大小

class Target
{
public:
    string title, words;
    int blg;
    double pos[2];//代表页面上的坐标，[0] 是横坐标，[1] 是纵坐标
    double width;//矩形宽度
    void to_shape_width()
    {
        this->width = this->words.length() > this->title.length() ? this->words.length() : this->title.length();
        this->width *= length_of_letter;
        this->width += length_of_edge;
    }
};

int t_in(string &a, string b, int b_start)
{
    int i = b_start;
    while(b[b_start] != ' ' && b[b_start] != ')')
    {   
        b_start ++;
    }
    a = b.substr(i, b_start - i);
    return b_start;
}

int the_len_of_each_level[max_len] = {0};
Target tar[max_len][max_level];

int In_a_level(string text,int i,int level,int the_len_of_last_tar)
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
    int level = -1;
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

    //搞定坐标和宽度
    for(int level = 0; level < max_level;level++)
    {
        for(int len = 0;len < max_len;len ++)
        {
            if(tar[len][level].title.length())//如果 tar 储存了信息
            {
                tar[len][level].to_shape_width();//宽度解决
                if(level == 0) tar[len][level].pos[1] = 0;//分以下四种情况
                if(len == 0) tar[len][level].pos[0] = 0;
                if(level)
                {
                    tar[len][level].pos[1] = tar[0][level-1].pos[1] + distance_of_level;
                }
                if(len)
                {
                    tar[len][level].pos[0] = tar[len-1][level].pos[0] + tar[len-1][level].width + distance_horizon;
                }
                //动态获取图片长宽
                pic_width = pic_width > (tar[len][level].pos[0] + tar[len][level].width) ? pic_width : (tar[len][level].pos[0] + tar[len][level].width);
                pic_height = pic_height > (level + 1)*distance_of_level ? pic_height : (level + 1)*distance_of_level;
            }
        }
    }

    //文件开始产生
    ofstream savefile("svg_user-defined.html");//创建文件以及句柄
    savefile << "<svg xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\""
            << pic_width
            << "\" height=\""
            << pic_height
            << "\">" << endl;

    //开始生成图形和文字
    for(int lv = 0; lv < max_level; lv ++)
    {
        for(int t = 0; t < max_len; t ++)
        {
            if(tar[t][lv].title.length())//如果该 tar 储存信息
            {
                //创建 group 标签
                savefile << "\t<g transform=\"translate(" 
                        << tar[t][lv].pos[0] 
                        << ", "  
                        << tar[t][lv].pos[1] 
                        << ")\">" 
                        <<endl;
                //创建 title 的 <rect> 标签
                savefile << "\t\t<rect x=\"0\" y=\"0\" height=\""
                        << rect_height
                        << "\" fill=\""
                        << color_fill_title
                        << "\" width=\""
                        << tar[t][lv].width
                        <<"\"></rect>" << endl;
                
                //创建 title 的 <text> 标签
                savefile << "\t\t<text font-size=\""
                        << font_size
                        << "\" x=\""
                        << title_pos[0]
                        << "\" y=\""
                        << title_pos[1]
                        << "\" fill=\""
                        << color_words
                        << "\" font-family=\""
                        << font_family
                        << "\">"
                        << tar[t][lv].title
                        << "</text>" << endl;
                
                //如果 tar 的 word 储存信息
                if(tar[t][lv].words.length())
                {
                    //创建 words 的 <rect> 标签
                    savefile << "\t\t<rect x=\"0\" y=\""
                            << rect_height
                            << "\" height=\""
                            << rect_height
                            << "\" fill=\""
                            << color_fill_words 
                            << "\" width=\""
                            << tar[t][lv].width
                            <<"\"></rect>" << endl;
                    
                    //创建 words 的 <text> 标签
                    savefile << "\t\t<text font-size=\""
                            << font_size
                            << "\" x=\""
                            << words_pos[0]
                            << "\" y=\""
                            << words_pos[1]
                            << "\" fill=\""
                            << color_words
                            << "\" font-family=\""
                            << font_family
                            << "\">"
                            << tar[t][lv].words
                            << "</text>" << endl;
                }
                //结束 group 标签
                savefile << "\t</g>" << endl << endl;
            }
            //开始生成线
            if(lv && tar[t][lv].title.length())
            {
                savefile << "<line stroke=\""
                        << color_line
                        <<"\""
                        <<" stroke-width=\"2\""
                        <<" x1=\""
                        <<tar[tar[t][lv].blg][lv-1].pos[0] + tar[tar[t][lv].blg][lv-1].width/2
                        <<"\""
                        <<" y1=\""
                        <<tar[tar[t][lv].blg][lv-1].pos[1] + rect_height
                        <<"\""
                        <<" x2=\""
                        <<tar[t][lv].pos[0] + tar[t][lv].width/2
                        <<"\""
                        <<" y2=\""
                        <<tar[t][lv].pos[1]
                        <<"\""
                        <<"></line>"<<endl;

            }
        }
    }

    //开始生成线

    savefile << "</svg>" << endl;
    savefile.close();//结束创建并关闭文件
}


#include <iostream>
#include "ChatRoom.h"
#include "HanldeMessage.h"

using namespace std;

#define MAX_LINE     8192

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout<<"缺少参数，请按照该格式运行本程序：程序名 服务器IP 服务器端口."<<endl;
        return -1;
    }

    CChatRoom Chat(argv[1], argv[2]);

    Chat.Start();

    cout<<"main"<<endl;
    return 0;
}

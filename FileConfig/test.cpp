#include "config_op.h"
#define MYFILE "e:/config_test.txt"

void display_menu() {
    //显示菜单
    cout << "==========================\n"; 
    cout << "退出测试：(0)\n";
    cout << "测试写配置文件：(1)\n";
    cout << "测试读配置文件：(2)\n";
    cout << "==========================\n";
}

int test_write()//测试写配置项
{
    char key[30];
    char value[30];
    int ret = 0;

    cout << "请输入key: \n";
    cin >> key;
    cout << "请输入value: \n";
    cin >> value;

    ret = wirte_config(MYFILE, key, value);

    if (ret != 0) {
        cout << "FUNCTION: wirte_config ERROR: " << ret << endl;
        return -1;
    }
    return ret;
}

int test_read() //测试读配置项
{
//    read_config(const char *filename/*in*/, const char *key/*in*/, char **value/*out*/)
    char key[30] = {0};
    char *value = NULL;
    int ret = 0;

    cout << "请输入key: \n";
    cin >> key;

    ret = read_config(MYFILE, key, &value);

    if (ret != 0) {
        cout << "FUNCTION: read_config ERROR: " << ret << endl;
        return -1;
    }
    if (value != NULL)
        cout << "键值对为" << key << " =" << value << endl;
    else
        cout << "没有键" << key << "的记录" << endl;
    return ret;
}


int main() {
    int choose;

    while (true) {
        display_menu();

        cin >> choose;
        switch (choose) {
            case 1:
                test_write();
                break;
            case 2:
                test_read();
                break;

            case 0:
                exit(0);
            default:
                cout << "请输入0,1,2: " << endl;
                continue;
        }

    }

    return 0;
}

#include <gtest/gtest.h>
#include <unistd.h>
#include <cstring>

using namespace std;

class GetoptTest: public testing::Test {
    public:
        enum {
            ARRSIZE = 200,
            STRLEN = 64,
        };

    protected:
        virtual void SetUp() {
            optind = 0;
            a_flag = false;
            code_count = 0;
            memset(codes, 0, sizeof(codes));
        }

        int parse_args(int argc, char *argv[]) {
            const char* optstring = "ak:";
            int opt = 0;
            while ((opt = getopt(argc, argv, optstring)) != -1) {
                switch (opt) {
                    case 'a':
                        a_flag = true;
                        break;
                    case 'k':
                        strncpy(codes[code_count], optarg, STRLEN);
                        code_count++;
                        break;
                    default:
                        break;
                }
            }
            return optind;
        }

        bool a_flag;
        int  code_count;
        char codes[ARRSIZE][STRLEN + 1];
};

TEST_F(GetoptTest, Test1) {
    // EXPECT_EQ(1, optind);

    int argc = 6;
    char *argv[20] = {"getopt_test", "-a", "-k", "apple", "-k", "banana", NULL};
    EXPECT_EQ(6, parse_args(argc, argv));
    EXPECT_EQ(true, a_flag);
    EXPECT_EQ(2, code_count);
    EXPECT_STREQ("banana", codes[1]);
    EXPECT_STREQ("apple", codes[0]);
}


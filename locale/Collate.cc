#include <stdio.h>
#include <wchar.h>
#include <xlocale.h>
#include <locale.h>

namespace TestDiff {
// mac下和linux下结果不同
// mac下的collate文件在 /usr/share/locale/en_US.UTF-8/
// linux下collate文件在 /usr/share/i18n/locales/en_US/
void run() {
    const char* curLocale = setlocale(LC_COLLATE, "");
    printf( "%s\n", curLocale);

    {
        setlocale( LC_COLLATE, "C" );
        int df = wcscoll( L"あい", L"いあ" );
        printf( "C diff: %d\n", df );
    }
    {
        setlocale( LC_COLLATE, "en_US.UTF-8" );
        int df = wcscoll( L"あい", L"いあ" );
        printf( "en_US.UTF-8: %d\n", df );
    }
    {
        setlocale( LC_COLLATE, "zh_CN.UTF-8" );
        int df = wcscoll( L"あい", L"いあ" );
        printf( "zh_CN.UTF-8: %d\n", df );
    }
}

}

int main() {
    TestDiff::run();
	return 0;
}

//Experiment P93:2

int compare(const int *a, int n, const int *b, int m) 
{
    int len = (n < m) ? n : m;
    //先比较公共位
    for (int i = 0; i < len; ++i) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return  1;
    }
    //公共位相同，比较位数
    if (n == m) return 0;
    return (n < m) ? -1 : 1;
}

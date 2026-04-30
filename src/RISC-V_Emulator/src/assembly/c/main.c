long write (int fd, const void *buf, int count);
long read (int fd, void *buf, int count);

int main() {
    void *buf;
    int n = read(0, buf, 100);
    write(1, buf, n);

    return 0;
}   
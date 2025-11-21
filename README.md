# 📁 FileSync Utility — 轻量级 Linux 文件同步工具
🔄 一个简洁可靠的文件夹同步工具，通过对比 Linux 的 ctime（inode 变化时间） 来判断文件更新状态，并自动生成新增文件列表。
适用于磁盘备份、U 盘同步、课程设计等场景。

## ✨ 项目亮点

🖥 跨目录同步：输入源目录（DS）和目标目录（DD）即可开始同步

🔍 自动文件对比：使用 inode 的 ctime 判断文件新旧

🤖 差异化更新：

  目标目录没有 → 自动复制

  源文件旧 → 询问是否覆盖

  源文件新 → 自动覆盖

📝 自动生成新增文件清单：写入 added_files.txt

💡 逻辑简单、代码清晰、易于扩展（可加递归、哈希校验、日志等）

## 📦 项目结构

```bash
FileSync/
│
├── filesync.c        # 主程序（C语言）
├── README.md         # 项目说明文档
└── added_files.txt   # 程序运行后生成（记录新增文件）
```

## 🚀 如何使用
1. 编译

```bash
gcc filesync.c -o filesync
```

2. 运行

```bash
./filesync
```

3. 输入源目录与目标目录
程序会提示你：

```bash
Enter source folder: DS
Enter destination folder: DD
```

随后立即开始文件同步。

## 🔧 主要功能模块解析
📌 1. 文件复制模块 copy_file()

使用缓冲区分块读写，实现二进制文件完整复制。

📌 2. 文件存在判断 file_exists()

基于 stat() 判断目标文件是否存在。

📌 3. 获取文件 ctime（inode 改变时间）get_ctime()

ctime 不是创建时间，而是 inode 信息最近修改的时间。

会在以下情况更新：

修改内容

更改权限

修改文件名

移动文件位置

非常适合用来比较“文件版本新旧”。

📌 4. 路径拼接 join_path()

确保目录后有 /，用于安全连接路径。

📌 5. 主循环：文件同步逻辑

对于源目录内每个文件：

情况	处理
目标目录没有该文件	复制 + 写日志
源文件 ctime < 目标 ctime	询问用户是否覆盖
源文件 ctime ≥ 目标 ctime	自动覆盖

同步结束后生成 added_files.txt。

## 📄 示例输出

同步过程中可能看到：

```bash
File: report.doc
Source ctime: 1700001200
Target ctime: 1700009999
Overwrite? (y/n):
```

新增文件清单示例：

```bash
DS/new_photo.jpg
DS/new_report.pdf
```

## 🛠 未来预计扩展功能

 支持递归同步子目录

 使用 MD5 判断文件内容变化

 增加详细日志输出

 添加 GUI 界面

 支持 Windows 平台

## 📜 License

推荐使用 MIT License：

```bash
MIT License
```

## ❤️ 致谢

本项目为课程实验作业开发，展示了 Linux 文件 I/O、目录遍历、时间戳比较等基础知识在实际工具中的应用。

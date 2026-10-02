Môn: IT003 - Cấu trúc dữ liệu và giải thuật


- report/: báo cáo (PDF)
- src/: mã nguồn C++
- data/: 10 bộ dữ liệu thử nghiệm (1.000.000 số thực/dãy)
- results.csv: kết quả đo thời gian (ms)


g++ -O2 -std=c++17 src/gen_data.cpp -o gen_data
./gen_data
g++ -O2 -std=c++17 src/sort_experiment.cpp -o sort_experiment
./sort_experiment
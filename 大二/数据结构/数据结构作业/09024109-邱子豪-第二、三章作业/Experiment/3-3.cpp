//P168:1
#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Plane {
    int id;             // 飞机编号
    int remaining_time; // 剩余飞行时间（用于落地队列）
    int arrive_time;    // 进入队列的时间
    bool isLanding;     // true为落地，false为起飞
};

class AirportSimulation {
public:
    AirportSimulation(int time_units, int max_planes_per_time, bool use_random_input = true)
        : time_units(time_units), max_planes_per_time(max_planes_per_time), use_random_input(use_random_input) {
        plane_id_landing = 0;
        plane_id_takeoff = 1;
        total_takeoff_wait = 0;
        total_landing_wait = 0;
        total_takeoff_planes = 0;
        total_landing_planes = 0;
        total_landing_no_fuel = 0;
        total_landing_flying_time = 0;
        srand((unsigned)time(0));
    }

    void run() {
        for (int t = 0; t < time_units; ++t) {
            cout << "========================= 时间单位 " << t << " =========================\n";

            // 获取输入
            int arriving_takeoff = use_random_input ? rand() % (max_planes_per_time+1) : getTakeoffInput(t);
            int arriving_landing = use_random_input ? rand() % (max_planes_per_time+1) : getLandingInput(t);

            vector<int> landing_times;
            for (int i = 0; i < arriving_landing; ++i) {
                landing_times.push_back(use_random_input ? rand() % 7 + 1 : getLandingTimeInput(t, i));
            }

            // 新到飞机进入队列
            distributeLandingPlanes(arriving_landing, landing_times, t);
            distributeTakeoffPlanes(arriving_takeoff, t);

            // 跑道服务（落地/起飞）
            serviceRunways(t);

            // 输出队列状态
            outputStatus(t);

            // 所有落地队列中的飞机剩余飞行时间减1
            updateLandingPlanes();
        }

        // 输出统计信息
        outputSummary();
    }

private:
    int time_units;
    int max_planes_per_time;
    bool use_random_input;

    queue<Plane> landing_queues[4]; // 4个落地队列
    queue<Plane> takeoff_queues[3]; // 3个起飞队列

    int plane_id_landing;
    int plane_id_takeoff;

    double total_takeoff_wait;
    double total_landing_wait;
    int total_takeoff_planes;
    int total_landing_planes;
    int total_landing_no_fuel;
    double total_landing_flying_time;

    void distributeLandingPlanes(int n, const vector<int>& flying_times, int current_time) {
        for (int i = 0; i < n; ++i) {
            int min_idx = 0;
            for (int j = 1; j < 4; ++j) {
                if (landing_queues[j].size() < landing_queues[min_idx].size())
                    min_idx = j;
            }
            Plane p{plane_id_landing, flying_times[i], current_time, true};
            landing_queues[min_idx].push(p);
            plane_id_landing += 2;
            cout << "飞机 " << p.id << " 到达落地队列，剩余飞行时间 " << p.remaining_time << "（队列 " << min_idx+1 << "）\n";
        }
    }

    void distributeTakeoffPlanes(int n, int current_time) {
        for (int i = 0; i < n; ++i) {
            int min_idx = 0;
            for (int j = 1; j < 3; ++j) {
                if (takeoff_queues[j].size() < takeoff_queues[min_idx].size())
                    min_idx = j;
            }
            Plane p{plane_id_takeoff, -1, current_time, false};
            takeoff_queues[min_idx].push(p);
            plane_id_takeoff += 2;
            cout << "飞机 " << p.id << " 到达起飞队列（队列 " << min_idx+1 << "）\n";
        }
    }

    void serviceRunways(int current_time) {
        vector<Plane> serviced;
        vector<string> serviced_type;
        int serviced_runway[3] = {0, 0, 0};

        // 第一步：优先无油落地
        vector<pair<int, int>> zero_fuel_planes;
        for (int i = 0; i < 4; ++i) {
            if (!landing_queues[i].empty() && landing_queues[i].front().remaining_time == 0) {
                zero_fuel_planes.push_back({i, 0});
            }
        }
        int zero_fuel_count = zero_fuel_planes.size();
        int serviced_count = 0;

        if (zero_fuel_count > 0) {
            for (int i = 0; i < zero_fuel_count && serviced_count < 3; ++i) {
                int runway = (zero_fuel_count == 1) ? 2 : serviced_count;
                int qidx = zero_fuel_planes[i].first;
                Plane p = landing_queues[qidx].front();
                landing_queues[qidx].pop();
                serviced.push_back(p);
                serviced_type.push_back("落地");
                serviced_runway[runway] = 1;
                serviced_count++;
                total_landing_no_fuel++;
                total_landing_wait += current_time - p.arrive_time;
                total_landing_planes++;
                total_landing_flying_time += p.remaining_time;
                cout << "优先无油落地：飞机 " << p.id << " 使用跑道 " << (runway+1) << " 落地\n";
            }
        }

        // 第二步：剩余跑道服务
        for (int runway = 0; runway < 3 && serviced_count < 3; ++runway) {
            if (serviced_runway[runway]) continue;

            if (runway == 2) {
                if (!takeoff_queues[runway].empty()) {
                    Plane p = takeoff_queues[runway].front();
                    takeoff_queues[runway].pop();
                    serviced.push_back(p);
                    serviced_type.push_back("起飞");
                    serviced_runway[runway] = 1;
                    serviced_count++;
                    total_takeoff_wait += current_time - p.arrive_time;
                    total_takeoff_planes++;
                    cout << "飞机 " << p.id << " 从跑道3起飞\n";
                }
            } else {
                int qidx = -1;
                if (!landing_queues[runway].empty()) qidx = runway;
                else if (!takeoff_queues[runway].empty()) qidx = -2;

                if (qidx >= 0) {
                    Plane p = landing_queues[qidx].front();
                    landing_queues[qidx].pop();
                    serviced.push_back(p);
                    serviced_type.push_back("落地");
                    serviced_runway[runway] = 1;
                    serviced_count++;
                    total_landing_wait += current_time - p.arrive_time;
                    total_landing_planes++;
                    total_landing_flying_time += p.remaining_time;
                    cout << "飞机 " << p.id << " 从落地队列 " << (qidx+1) << " 在跑道 " << (runway+1) << " 落地\n";
                } else if (qidx == -2) {
                    Plane p = takeoff_queues[runway].front();
                    takeoff_queues[runway].pop();
                    serviced.push_back(p);
                    serviced_type.push_back("起飞");
                    serviced_runway[runway] = 1;
                    serviced_count++;
                    total_takeoff_wait += current_time - p.arrive_time;
                    total_takeoff_planes++;
                    cout << "飞机 " << p.id << " 从起飞队列 " << (runway+1) << " 在跑道 " << (runway+1) << " 起飞\n";
                }
            }
        }
    }

    void updateLandingPlanes() {
        for (int i = 0; i < 4; ++i) {
            int sz = landing_queues[i].size();
            for (int j = 0; j < sz; ++j) {
                Plane p = landing_queues[i].front();
                landing_queues[i].pop();
                if (p.remaining_time > 0) p.remaining_time--;
                landing_queues[i].push(p);
            }
        }
    }

    void outputStatus(int current_time) {
        cout << "--- 队列状态（时间 " << current_time << "） ---\n";
        for (int i = 0; i < 4; ++i) {
            cout << "落地队列 " << (i+1) << ": ";
            printQueue(landing_queues[i]);
        }
        for (int i = 0; i < 3; ++i) {
            cout << "起飞队列 " << (i+1) << ": ";
            printQueue(takeoff_queues[i]);
        }
        cout << endl;
    }

    void printQueue(queue<Plane> q) {
        if (q.empty()) {
            cout << "[空]\n";
            return;
        }
        while (!q.empty()) {
            Plane p = q.front();
            q.pop();
            if (p.isLanding)
                cout << "(编号:" << p.id << ",剩余:" << p.remaining_time << ") ";
            else
                cout << "(编号:" << p.id << ") ";
        }
        cout << endl;
    }

    void outputSummary() {
        cout << "\n========================= 仿真统计信息 =========================\n";
        cout << "落地飞机总数: " << total_landing_planes << endl;
        cout << "起飞飞机总数: " << total_takeoff_planes << endl;
        cout << "平均落地等待时间: " << (total_landing_planes ? total_landing_wait/total_landing_planes : 0) << endl;
        cout << "平均起飞等待时间: " << (total_takeoff_planes ? total_takeoff_wait/total_takeoff_planes : 0) << endl;
        cout << "落地飞机剩余飞行时间平均值: " << (total_landing_planes ? total_landing_flying_time/total_landing_planes : 0) << endl;
        cout << "无油落地飞机数量: " << total_landing_no_fuel << endl;
    }

    // 手动输入函数接口（如需自定义输入可完善）
    int getTakeoffInput(int t) { return 0; }
    int getLandingInput(int t) { return 0; }
    int getLandingTimeInput(int t, int i) { return 1; }
};

int main() {
    // 仿真5个时间单位，每单位最多3架飞机，使用随机数据
    AirportSimulation sim(5, 3, true);
    sim.run();
    return 0;
}
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <vector>
#include <map>


enum class HouseType {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    Count = 4,
    R = 254,       // 任意随机
    S = 255        // 随机，但同容器内的S必须一致，例如 [ S A A S ] 只能有4种
};

std::map<HouseType, const char*> Name = {{HouseType::A, "A"}, {HouseType::B, "B"},
                                         {HouseType::C, "C"}, {HouseType::D, "D"},
                                         {HouseType::R, "R"}, {HouseType::S, "S"}};

std::vector<double> Area = {80.0, 100.0, 120.0, 140.0};

struct Conditions {
    double totalArea;                  // 总面积
    double areaTolerance;              // 面积允许误差
    double portionTolerance;           // 比例允许误差
    std::vector<double> housePortions; // 各个房型的面积占总面积的比例
    int64_t mask;                      // 房型的集合

    Conditions() : housePortions((int)HouseType::Count, 0.0) {}
    void add(HouseType t, double portion) {
        housePortions[(int)t] = portion;
        if(portion > 0) mask |= (1LL << (int)t);
    }
    bool hasHouse(HouseType t) {
        return mask & (1LL << (int)t);
    }
};

Conditions cond;
std::vector< std::vector< HouseType > > TemplateContainer; // 所有可能的容器，可能包含R，S这种通配
std::vector< std::vector< HouseType > > ConfigContainer;   // 配置可选的容器，可能包含R，S这种通配
std::vector< std::vector< HouseType > > ValidContainer;    // 扩展后具体的容器，不包含通配
std::map<std::vector<int>, std::pair<double, std::vector<int> > > bestResult;

void outputContainer(const std::vector< HouseType > &container) {
    printf("[ ");
    for(auto type : container) printf("%s ", Name[type]);
    printf("]");
}

void outputResult(const std::vector<int> &usedIndex, const std::vector<int> &count) {
    std::vector<int> typeCount((int)HouseType::Count, 0);
    for(int i = 0; i < usedIndex.size(); ++i) {
        for(auto type : ValidContainer[usedIndex[i]]) {
            typeCount[(int)type] += count[i];
        }
    }
    double total = 0.0;
    for(int i = 0; i < (int)HouseType::Count; ++i) {
        total += Area[i] * typeCount[i];
    }

    printf("Got one\n\t");
    for(int i = 0; i < (int)HouseType::Count; ++i) {
        if(cond.hasHouse((HouseType)i)) {
            printf("%s(%.3f) ", Name[(HouseType)i], Area[i] * typeCount[i] / total);
        }
    }
    printf("\n");
    for(int i = 0; i < usedIndex.size(); ++i) {
        printf("\t");
        outputContainer(ValidContainer[usedIndex[i]]);
        printf(" * %d\n", count[i]);
    }
}

void dfs(const std::vector< HouseType > &input, size_t pos, std::vector< HouseType > &output, HouseType sValue) {
    if(pos >= input.size()) {
        ValidContainer.push_back(output);
        return;
    }
    HouseType t = input[pos];
    if(t == HouseType::R) {
        for(int i = 0; i < (int)HouseType::Count; ++i) {
            output[pos] = (HouseType)i;
            dfs(input, pos + 1, output, sValue);
        }
    }
    else if(t == HouseType::S) {
        if(sValue == HouseType::S) {
            for(int i = 0; i < (int)HouseType::Count; ++i) {
                output[pos] = (HouseType)i;
                dfs(input, pos + 1, output, (HouseType)i);
            }
        }
        else {
            output[pos] = sValue;
            dfs(input, pos + 1, output, sValue);
        }
    }
    else {
        output[pos] = t;
        dfs(input, pos + 1, output, sValue);
    }
}

// 根据配置，扩展出所有可能的容器
void expandContainer() {
    for(auto &c : ConfigContainer) {
        std::vector<HouseType> tmp(c.size(), HouseType::R);
        dfs(c, 0, tmp, HouseType::S);
    }
}

void initConditionsForTest() {
    cond.totalArea = 10000.0;
    cond.areaTolerance = 0.01;
    cond.portionTolerance = 0.02;
    cond.add(HouseType::A, 0.4);
    cond.add(HouseType::B, 0.3);
    cond.add(HouseType::C, 0.2);
    cond.add(HouseType::D, 0.1);
}

void initConditionsByHand() {
    printf("Please input total area: ");
    scanf("%lf", &cond.totalArea);

    printf("Please input area tolerance: ");
    scanf("%lf", &cond.areaTolerance);

    printf("There are %zd house types:\n", Area.size());
    for(int i = 0; i < (int)Area.size(); ++i) printf("\t%d - %s %.2f\n", i + 1, Name[(HouseType)i], Area[i]);

    while(true) {
        int type;
        double portion;
        printf("Please input house type and portion (0 0 for quit): ");
        scanf("%d %lf", &type, &portion);
        if (type == 0) break;
        cond.housePortions[type - 1] = portion;
        cond.mask |= (1LL << (type - 1));
    }

    printf("Please input portion tolerance: ");
    scanf("%lf", &cond.portionTolerance);


}

void initTemplateContainers() {
    TemplateContainer.push_back({HouseType::R, HouseType::R});
    TemplateContainer.push_back({HouseType::S, HouseType::S});
    TemplateContainer.push_back({HouseType::R, HouseType::A, HouseType::R});
    TemplateContainer.push_back({HouseType::R, HouseType::B, HouseType::R});
    TemplateContainer.push_back({HouseType::R, HouseType::C, HouseType::R});
    TemplateContainer.push_back({HouseType::R, HouseType::D, HouseType::R});
    TemplateContainer.push_back({HouseType::S, HouseType::A, HouseType::S});
    TemplateContainer.push_back({HouseType::S, HouseType::B, HouseType::S});
    TemplateContainer.push_back({HouseType::S, HouseType::C, HouseType::S});
    TemplateContainer.push_back({HouseType::S, HouseType::D, HouseType::S});
    TemplateContainer.push_back({HouseType::R, HouseType::A, HouseType::A, HouseType::R});
    TemplateContainer.push_back({HouseType::R, HouseType::B, HouseType::B, HouseType::R});
    TemplateContainer.push_back({HouseType::R, HouseType::C, HouseType::C, HouseType::R});
    TemplateContainer.push_back({HouseType::R, HouseType::D, HouseType::D, HouseType::R});
    TemplateContainer.push_back({HouseType::S, HouseType::A, HouseType::A, HouseType::S});
    TemplateContainer.push_back({HouseType::S, HouseType::B, HouseType::B, HouseType::S});
    TemplateContainer.push_back({HouseType::S, HouseType::C, HouseType::C, HouseType::S});
    TemplateContainer.push_back({HouseType::S, HouseType::D, HouseType::D, HouseType::S});
}

void initContainersForTest() {

    ConfigContainer.push_back({HouseType::R, HouseType::R});
    ConfigContainer.push_back({HouseType::R, HouseType::A, HouseType::R});
    ConfigContainer.push_back({HouseType::R, HouseType::B, HouseType::R});
    ConfigContainer.push_back({HouseType::R, HouseType::C, HouseType::R});
    ConfigContainer.push_back({HouseType::R, HouseType::D, HouseType::R});
    ConfigContainer.push_back({HouseType::R, HouseType::A, HouseType::A, HouseType::R});
    ConfigContainer.push_back({HouseType::R, HouseType::B, HouseType::B, HouseType::R});
    ConfigContainer.push_back({HouseType::R, HouseType::C, HouseType::C, HouseType::R});
    ConfigContainer.push_back({HouseType::R, HouseType::D, HouseType::D, HouseType::R});

    expandContainer();
}

void initContainersByHand() {
    printf("There are %zd template container types:\n", TemplateContainer.size());
    for(int i = 0; i < (int)TemplateContainer.size(); ++i) {
        printf("\t%d - ", i + 1);
        outputContainer(TemplateContainer[i]);
        printf("\n");
    }
    printf("Please select container index (0 for quit): ");
    while(true) {
        int idx;
        scanf("%d", &idx);
        if(idx == 0) break;
        ConfigContainer.push_back(TemplateContainer[idx - 1]);
    }

    expandContainer();
}

// 获取所有房型集合
int64_t getMask(const std::vector<int> &usedIndex) {
    int64_t mask = 0;
    for(auto idx : usedIndex) {
        for(auto type : ValidContainer[idx]) {
            mask |= (1LL << (int)type);
        }
    }
    return mask;
}



void dfs3(const std::vector<int> &usedIndex, size_t pos, std::vector<int> &count) {
    if(pos == usedIndex.size()) {
        std::vector<int> containerCount(usedIndex.size(), 0);
        std::vector<int> houseCount((int)HouseType::Count, 0);

        // 计算当前总面积
        double totalArea = 0.0;
        for(int i = 0; i < usedIndex.size(); ++i) {
            double containerArea = 0.0;
            for(auto type : ValidContainer[usedIndex[i]]) {
                containerArea += Area[(int)type];
            }
            totalArea += containerArea * count[i];
        }
        // 根据比例缩放容器数量
        double ratio = cond.totalArea / totalArea;
        for(int i = 0; i < usedIndex.size(); ++i) {
            containerCount[i] = std::round(count[i] * ratio);
        }
        // 按缩放后的数量再次统计
        totalArea = 0.0;
        for(int i = 0; i < usedIndex.size(); ++i) {
            double containerArea = 0.0;
            for(auto type : ValidContainer[usedIndex[i]]) {
                containerArea += Area[(int)type];
                houseCount[(int)type] += containerCount[i];
            }
            totalArea += containerArea * containerCount[i];
        }
        // 总面积不在允许范围内
        if(totalArea < cond.totalArea || totalArea > cond.totalArea * (1.0 + cond.areaTolerance)) {
            return;
        }

        // 查看各种房型的面积所占总面积的比例是否在允许误差内
        double sumError = 0.0; // 每种房型误差的绝对值的和，这个作为一个评价指标
        bool ok = true;
        for(int i = 0; i < (int)HouseType::Count; ++i) {
            if(cond.hasHouse((HouseType)i)) {
                double err = fabs( Area[i] * houseCount[i] / totalArea - cond.housePortions[i]);
                if(err > cond.portionTolerance) {
                    ok = false;
                    break;
                }
                sumError += err;
            }
        }
        if(ok) {
            auto it = bestResult.find(usedIndex);
            if(it != bestResult.end()) {
                if(sumError < it->second.first) {
                    it->second.first = sumError;
                    it->second.second = containerCount;
                }
            }
            else {
                bestResult.insert(std::make_pair(usedIndex, std::make_pair(sumError, containerCount)));
            }

            // 这里结果太多，只输出100种结果就退出了
            if(bestResult.size() > 100) {
                for(auto &kv : bestResult) {
                    outputResult(kv.first, kv.second.second);
                }
                exit(0);
            }
        }
        return;
    }
    for(int c = 1; c <= 30; ++c) {
        count[pos] = c;
        dfs3(usedIndex, pos + 1, count);
    }
}

void calc(const std::vector<int> &usedIndex) {
    int64_t mask = getMask(usedIndex);
    if(mask != cond.mask) return;
    std::vector<int> count(usedIndex.size(), 0);
//    printf("try ");
//    for(auto idx : usedIndex) printf("%d ", idx);
//    printf("\n");
    // 根据已经选择的容器，枚举所有数量
    dfs3(usedIndex, 0, count);
}

void dfs2(std::vector<int> &usedIndex, size_t pos, int searchBegin) {
    if(pos == usedIndex.size()) {
        calc(usedIndex);
        return;
    }
    if(searchBegin >= ValidContainer.size()) {
        return;
    }
    for(int i = searchBegin; i < ValidContainer.size(); ++i) {
        usedIndex[pos] = i;
        dfs2(usedIndex, pos + 1, i + 1);
    }
}

void solve() {
    printf("Calculating...\n");
    // 根据容器种类数，枚举所有可能的容器的组合
    for(int count = 2; count <= 4; ++count) {
        std::vector<int> usedIndex(count, 0);
        dfs2(usedIndex, 0, 0);
    }
}

int main(int argc, char** argv) {

    initTemplateContainers();

    if(argc >= 1) {
        initConditionsForTest();
        initContainersForTest();
    }
    else {
        initConditionsByHand();
        initContainersByHand();
    }

    solve();

    return 0;
}
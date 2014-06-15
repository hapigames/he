#ifndef __BUILD_H__
#define __BUILD_H__


class BuildInf {
public:
    BuildInf(): id_(0), owner_uid_(0),level_(0),mid_(0),position_(0),gem_type_(0) {}
    BuildInf(int id, int ouid, int lv, int mid, int pos, int gt): 
        id_(id), owner_uid_(ouid),level_(lv),mid_(mid),position_(pos),gem_type_(gt) {}
    long long id_;
    long long owner_uid_;
    int level_;
    int mid_;
    int position_;
    int gem_type_;
};

#endif // __BUILD_H__

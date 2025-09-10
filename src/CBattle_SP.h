#pragma once

// �X�^�[�g�X�L���|�C���g
#define START_SP	3
// MAX�X�L���|�C���g�i�萔�j
#define MAX_SP	5

class BattleSP {
private:

    // ���݂�SP
    int SP = START_SP;
    // �ő�SP
    const int maxSP = MAX_SP;

public:
    // �X�L���g�p�\���`�F�b�N
    bool comfirmSP() const { return this->SP > 0; }
    // ���݂�SP�̎擾
    int getSP() const { return SP; }
    // �ő�SP�̎擾
    int getMaxSP() const { return maxSP; }


    // �X�L���g�p���ASP�̏���
    void consumeSP() {
        if (!comfirmSP()) { return; }
        SP--;
    }

    // �ʏ�U�����ASP�̏㏸
    void recoverSP() {
        if (SP < maxSP) { SP++;}
    }

    // SP�̏�����
    void ResetSP() { SP = START_SP; }

};
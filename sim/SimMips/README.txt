/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS    Arch Lab. TOKYO TECH *
 **********************************************************************/
COPYRIGHT

SimMips is free software.
See the file COPYING for copying permission.

/**********************************************************************/
/* 0. Index                                                           */
/**********************************************************************/
 1. About SimMips
 2. Building your SimMips binary  
 3. Let's run SimMips  
 4. Various modes of SimMips
 5. Command Line Options
 6. Machine Setting File
 7. SimMips History
 8. Project
 9. Reference

/**********************************************************************/
/* 1. About SimMips                                                   */
/**********************************************************************/
�v�Z�@�A�[�L�e�N�`����I�y���[�e�B���O�V�X�e���̋���E�����c�[���Ƃ��āC
�V�X�e���V�~�����[�^�ւ̗v�������܂��Ă��܂��D�������Ȃ���C�����̃V�X
�e���V�~�����[�^�́C�������̃v���b�g�t�H�[���ɑΉ����邱�Ƃ�C�����ɓ�
�삷�邱�ƂɎ��������Ă��邽�߁C����E�����ɂ����ďd�v�ƂȂ�V���v��
����ǐ��������Ă��܂��D

�������������̃V�~�����[�^�Ƃ͈�����悵�C�V���v������ǐ��C�g������
�d�������CMIPS32���߃Z�b�g�̃v���Z�b�T���܂ރV�X�e���V�~�����[�^�ł���
SimMips �̊J���������Ȃ��Ă��܂��D�ǐ��Ǝ��s���x�Ƃ̊Ԃɂ̓g���[�h�I
�t������܂����C�v���Z�b�T�̍������ɂ��C�����ǐ��ƌ����I�Ȏ��s���x
�Ƃ̗������\�ƂȂ����܂��DSimMips �́C�v�Z�@�V�X�e����OS�̋���E
�����E�J���ɑ傢�ɗ��_�𔭊����܂��D

/**********************************************************************/
/* 2. Building your SimMips binary                                    */
/**********************************************************************/
SimMips�̓N���[����C++�ŋL�q����Ă��܂��DIntel Linux, Intel Cygwin��
�ǂ̊��œ�����m�F���Ă��܂��D�i�������C�ꕔ���[�h�ł�X�ɑΉ����Ă�
��K�v������܂��j
�����_�ł�Big-Endian�̃v���b�g�t�H�[���C�Ⴆ��Cell/B.E. Linux �Ȃǂ̊�
���ł͓��삵�܂���D

SimMips �̎��s�t�@�C�����쐬���邽�߂ɂ̓R���p�C�����K�v�ł��D
SimMips �̔z�z�p�b�P�[�W���_�E�����[�h���āC�W�J���܂��D
�i���̃t�@�C����ǂ�ł���Ƃ������Ƃ́C���Ƀp�b�P�[�W���W�J����Ă���
�@��������܂��񂪁D�j

�Ⴆ�΁C�t�@�C������SimMips-0.6.6.tgz �Ȃ�΁C�ȉ��̃R�}���h�Ńp�b�P�[
�W��W�J���܂��D

$ tar xvzf SimMips-0.6.6.tgz

SimMips-0.6.6 �Ƃ����f�B���N�g�����쐬����C���̒��Ƀ\�[�X�R�[�h�Ȃ�
���W�J����܂��D���ɁC�W�J���ꂽ�f�B���N�g���Ɉړ����ăR���p�C�����܂��D

$ cd SimMips-0.6.6
$ make

�R���p�C��������ɏI������΁Cbin�f�B���N�g���̒��� SimMips �ق�������
���̎��s�t�@�C�����o���Ă���͂��ł��D

$ ls bin/
SimMieru  SimMips  SimMipsF  SimMipsM  SimMipsOS

/**********************************************************************/
/* 3. Let's run SimMips                                               */
/**********************************************************************/
�p�����[�^���w�肹����SimMips �����s����ƁC�ȒP�Ȏg�������\������܂��D

| $ ./bin/SimMips
| ## SimMips: Simple Computer Simulator of MIPS Version 0.6.6 2010-07-02
| Usage: SimMips [-options] object_file_name
|  [Basic Options]
|   -e[num][kmg]: stop simulation after num cycles executed
|   -k[num][kmg]: skip first num cycles
|   -d1: debug mode 1: put the architectural state after simulation
|   -d2: debug mode 2: put all the instructions executed
|   -d3: debug mode 3: put the trace of the execution
|   -i: put the instruction mix after simulation
|   -M [filename]: specify machine setting file

�T���v���Ƃ��āC�N�C�b�N�\�[�g�̃v���O������test/qsort�ɒu����Ă��܂��D
�܂��́C���̃v���O������SimMips �œ��삳���Ă݂܂��傤�D

| $ ./bin/SimMips test/qsort
| ## SimMips: Simple Computer Simulator of MIPS Version 0.6.6 2010-07-02
|
| Before:   65536 items quick sorting.
| a[    0] =        9086
| a[  200] =       28147
| a[  400] =        7617
| a[  600] =        9586
| a[  800] =       18764
| a[ 1000] =       28363
| a[ 1200] =       23597
| a[ 1400] =       26600
| a[ 1600] =        6196
| a[ 1800] =       12923
| 
| After:   65536 items quick sorting.
| a[    0] =           0
| a[  200] =          98
| a[  400] =         197
| a[  600] =         300
| a[  800] =         402
| a[ 1000] =         503
| a[ 1200] =         600
| a[ 1400] =         701
| a[ 1600] =         802
| a[ 1800] =         897
| 
| ## cpu stopped
| ## cycle count: 17663749
| ## inst count: 17663749
| ## simulation time:    1.431
| ## mips: 12.343

��̂悤�ȃ��b�Z�[�W���\������܂������H
���s���ԂȂ�(��ԉ���2�s) �́C���p���Ă���v�Z�@�ɂ���ĕω����܂��D

/**********************************************************************/
/* 4. Various modes of SimMips                                        */
/**********************************************************************/
�e���[�h��()����(sim���N�_�Ƃ����\�[�X�R�[�h�̃f�B���N�g��/���������
�o�C�i����)��\���Ă��܂��D

- App���[�h (core/SimMips)
�ÓI�Ƀ����N���ꂽMIPS�A�v���P�[�V���������s���邽�߂̃��[�h�ł��D�܂��C
���̃��[�h���\�����邽�߂̊�b�ƂȂ镔������������Ă��܂��D

- MultiCycle���[�h (multicycle/SimMipsM)
App���[�h�Ǝ��s�ł���A�v���P�[�V�����͓���ł����C1���߂𕡐��T�C�N��
�����Ď��s����}���`�T�C�N���̎��s���f�������܂��Dv0.5.x�ȑO�ɂ�����
-m�I�v�V�����ɑ���������̂ł��D

- OS���[�h (osmode/SimMipsOS)
OS�J�[�l���ƃf�B�X�N�C���[�W���w�肵�āCLinux�Ȃǂ�OS�𓮍삳���郂�[
�h�ł��DMIPS�v���Z�b�T����(CP0)�Ɗ����݃R���g���[���C�V���A���R���g��
�[���Ȃǂ̓���̃V�~�����[�g���܂܂�܂��D�ȉ���URL����_�E�����[�h��
����mipsel�p�f�B�X�N�C���[�W���𓀂������̂�image�f�B���N�g���ɒu���C
mem_qemu.txt���}�V���Z�b�e�B���O�t�@�C���ɁCvmlinux-2.6.18-3-qemu����
�s�o�C�i���Ƃ��Ď��s���邱�ƂŁC������m�F���邱�Ƃ��ł��܂��D
�@http://wiki.qemu.org/Download

- Float���[�h (float/SimMipsF)
App���[�h��FPU���ǉ����ꂽ�C���������_�������A�v���P�[�V���������s����
���߂̃��[�h�ł��D

- MieruPC���[�h (mieru/SimMieru)
MieruPC-2008�C�����2010�Ŏ��s�\�ȃA�v���P�[�V�����C���邢��OS�����s
���邽�߂̃��[�h�ł��D�t���f�B�X�v���C�̃V�~�����[�V�����ɂ�MieruSDK��
�ɓ����̉t���V�~�����[�^�𗘗p���܂��D�t���V�~�����[�^�𓮍삳���邽��
�ɂ�X���K�v�ł��D���̃��[�h�́Cv0.5.x�ȑO��App���[�h�̈ꕔ�Ƃ��Ē񋟂�
��Ă������̂��g���������̂ł��D

�� MieruSDK�́CMieruPC�v���W�F�N�g��Web�T�C�g�ɂČ��J�\��ł��D
�@http://www.arch.cs.titech.ac.jp/mieru/

/**********************************************************************/
/* 5. Command Line Options                                            */
/**********************************************************************/
-e�I�v�V�����́C���T�C�N���̎��s����������ƃV�~�����[�V�������I����
��I�v�V�����ł��D�ڔ����Ƃ��āCk �܂���K(1,000)�Cm �܂���M(100��)�C
g �܂���G(10��) �����p�ł��܂��D

| $ ./SimMips -e10k test/qsort
| ## SimMips: Simple Computer Simulator of MIPS Version 0.5.0 2008-11-05
| 
| ## cycle count reaches the limit
| ## cycle count: 10000
| ## inst count: 10000
| ## simulation time:    0.001
| ## mips: 11.455

-k�I�v�V�����́C���T�C�N���̎��s����������܂ł̊ԏo�͂�}������I�v
�V�����ł��D�}�������o�͂ɂ̓f�o�b�O���[�h�̏o�͂��܂݂܂��D

-d�I�v�V�����́C�f�o�b�O���[�h�ł��D1�`4�܂ł̃��[�h���w�肵�܂��D
-d1 �́C�V�~�����[�V�����I�����Ƀ��W�X�^�l�ƃ������̓��e��\�����܂��D
-d2 �́C���s���ꂽ���ߗ��\�����܂��D
-d3 �́C���s���ꂽ���ߗ�Ɠǂݏ������ꂽ���W�X�^�̒l��\�����܂��D
-d4 �́C����������O��TLB �̓ǂݏ���������\�����܂�(OS���[�h�̂�)�D

-i�I�v�V�����́C�V�~�����[�V�����I�����Ɏ��s���ꂽ���߂̎�ނƉ񐔂�\
�����܂��D�g�p���ꂽ�񐔂��������ɕ��בւ����܂��D

-M�I�v�V�����́C�������⃌�W�X�^�̏����ݒ�p�̃t�@�C�����w�肵�܂��D
�ڍ׎d�l�͌�q���܂��D

-l�I�v�V�����́CMieruPC���[�h�Ŏg�p���܂��D�t���V�~�����[�^�̃p�C�v��
�p�X���w�肵�܂��D

-t�I�v�V�����́CMieruPC���[�h�Ŏg�p���܂��D�A�v���P�[�V����(��OS)���z
�肷��t�����j�b�g�̎�ނ��w�肵�܂��D-t0��MieruPC-2008�̉t�����j�b�g�C
-t1��MieruPC-2010�̉t�����j�b�g��z�肵�܂��D

/**********************************************************************/
/* 6. Machine Setting File                                            */
/**********************************************************************/
�}�V���Z�b�e�B���O�t�@�C����-M�I�v�V�����Ŏw�肵�܂��D���L�̃t�H�[�}�b
�g�ɏ]���ċL�q���ꂽ�e�L�X�g�t�@�C����ǂݍ��܂��邱�ƂŁC���W�X�^�l��
�������}�b�v�C�������̏����l�Ȃǂ��w�肷�邱�Ƃ��o���܂��D

�w�b�_(1�s��)�́uSimMips_Machine_Setting�v�ŌŒ�ł��D
2�s�ڈȍ~�́C@�Ŏn�܂�s���R�}���h�Ƃ��Ĉ����C����ȊO�̓R�����g�ł��D

��@reg $REG=VALUE ���W�X�^�l�̎w��
��) @reg $a0=0x12345678
���W�X�^����$4�̂悤�Ȑ����ł��C$a0�̂悤��(������)�ʖ��ł��\���܂���D
PC��$pc�C�܂���$32�Ǝw�肵�܂��D

��@map BASE SIZE NAME �����������}�b�v�̎w��
��) @map 0x0 0x1000000 MAIN_MEMORY
�A�h���X���d������ꍇ�͐�ɏ����������D�悳��܂��D

NAME�Ŏw��\�Ȗ��O�͈ȉ��̒ʂ�ł��D
���O        �Ώۃ��[�h�@ �T�v
MAIN_MEMORY ���ׂ�       ���C��������
ISA_IO      OS           ISA�o�X��IO(���݂͊��荞�݂ƃV���A�����o��)
ISA_BUS     OS           ISA�o�X(���݂͋�̎���)
MIERU_IO    MieruPC      MieruPC��IO
MMC_IO      MieruPC      �}���`���f�B�A�J�[�h�̃o�X

��@mem ADDR NAME ���C���������ւ̃t�@�C����������
��) @mem 0x00010000 hoge.bin
ADDR�Ŏw�肵���A�h���X�ȍ~��NAME�Ŏw�肵���t�@�C���̒��g���������݂܂��D

��@mmc SIZE NAME �}���`���f�B�A�J�[�h�̎w��(MieruPC���[�h�̂�)
��) @mmc 1m image.dat
�}���`���f�B�A�J�[�h�Ƃ��āC�T�C�YSIZE�̂��̂�z�肵�܂��DSIZE�ɂ͐ڔ�
��k, m, g���w��ł��܂��D�J�[�h���̏����f�[�^�́CNAME�Ŏw�肳�ꂽ�t�@
�C������ǂݍ��݂܂��DNAME���w�肵�Ȃ������ꍇ�͏����f�[�^�̓[���Ŗ���
���܂��D�܂��CNAME�Ŏw�肵���t�@�C���̃T�C�Y��SIZE�����̏ꍇ�C�c���
�����̓[���Ŗ��߂��܂��D

/**********************************************************************/
/* 7. SimMips History                                                 */
/**********************************************************************/
��v�ȃo�[�W�������L�ڂ��Ă��܂��D

v0.6.7 2010-07-02 (3,110 + 4,427 lines)
�EMieruPC���[�h��MMC�C���[�W�̃t�@�C�����֘A�Ƀo�O���������̂��C���D

v0.6.6 2010-07-02 (3,110 + 4,409 lines)
�E�ꕔ�̕��������_���߂��������f�R�[�h�ł��Ȃ������s����C���D

v0.6.5 2010-06-30 (3,111 + 4,409 lines)
�E�}�V���Z�b�e�B���O�t�@�C���ɋL�q����p�X�̎w����@���C���݂̍�ƃf�B
�@���N�g���ł͂Ȃ��C�t�@�C�����g����̑��΃p�X�Ƃ����D

v0.6.4 2010-06-17 (3,084 + 4,409 lines)
�E���߃g���[�X�@�\�i-d2�܂���-d3�j�Ɋ֐��V���{���̕\����ǉ��D

v0.6.3 2010-06-14 (3,051 + 4,409 lines)
�Emieru��MieruPC2008�����̃A�v���P�[�V�����ւ̑Ή���ǉ��D
�i-t0�I�v�V���������Ď��s���Ă��������j

v0.6.2 2010-06-11 (3,051 + 4,233 lines)
�EMieruPC�ɑΉ�����t���@�\(mieru)��ǉ��D
�i���݂�MieruPC2010�����t���̂ݑΉ��j

v0.6.1 2010-06-10 (3,022 + 3,164 lines)
�E���������_���߂ɑΉ�����t���@�\(float)��ǉ��D

v0.6.0 2010-06-02 (3,026 + 1,563 lines)
�E�����\����啝�ɍ��V�D
�@��{�@�\(App�C�@�\���x��)��core�C�t���@�\�����̊g���Ƃ�����@�ɁD
�@�}���`�T�C�N����multicycle�COS���[�h��osmode�ɂ܂Ƃ߂Ă���D
�@MieruPC�����̊g���͂܂��������D

v0.5.5 2009-05-13 (4,540 lines)
�EMieruPC-2008�ւ̑Ή��C����уo�O�t�B�b�N�X�D

v0.5.0 2008-11-05 (4,422 lines)
�EMieruPC �����ɕʂŊJ�����Ă����o�[�W�����𓝍��Dv0.5.0�Ƃ��Č��J�D

v0.4.9 2008-08-13 (4,090 lines)
�E�\�[�X�R�[�h�̌�������C�@�\�̒ǉ��Ȃǂ𐏎��s���Ă����D

v0.4.0 2008-06-17 (3,765 lines)
�EOS���[�h�̃o�O��肪�I���D�V�F���������オ��Cls�Ȃǂ̓�����m�F�D

v0.3.1 2008-06-04 (3,459 lines)
�E���荞�݃R���g���[���ƃV���A���C���^�t�F�[�X�������D
�@�ڍׂȌ��؂͂��Ă��Ȃ��D

v0.3.0 2008-05-27 (2,980 lines)
�ECoprocessor 0(CP0)�������D�ڍׂȌ��؂͂��Ă��Ȃ��D

v0.2.0 2008-04-14 (2,384 lines)
�EApp ���[�h�̎����I���Dnqueen�v���O����������D

/**********************************************************************/
/* 8. Project                                                         */
/**********************************************************************/
SimMips �̊J���́CMieruPC �v���W�F�N�g�i���g��������v�Z�@�V�X�e�����\
�z���錤���E����v���W�F�N�g�j�̈�Ƃ��Ă��s���Ă��܂��D
MieruPC �v���W�F�N�g�Ɋւ�����͈ȉ���URL�Ɍf�ڂ���Ă��܂��D
�@http://www.arch.cs.titech.ac.jp/mieru/
MieruPC�̃n�[�h�E�F�A�Ɋւ��Ă͈ȉ���URL�Ɍf�ڂ���Ă��܂��D
�@http://www.mierupc.com/

�܂��CSimMips�̊J���̈ꕔ�́C�Ɨ��s���@�l�Ȋw�Z�p�U���@�\(JST) �헪�I
�n���������i����(CREST)�̎x�����󂯂Ă��܂��D

/**********************************************************************/
/* 9. Reference                                                       */
/**********************************************************************/
[1] ���}���P, �n糐L��, �g������F ����E�����ɗL�p��MIPS�V�X�e���V�~
�����[�^SimMips, ��񏈗��w��_����, Vol.50, No.11, pp. 2665-2676. 
(November 2009)

[2] Naoki Fujieda, Takefumi Miyoshi, and Kenji Kise: SimMips: A MIPS 
System Simulator, Workshop on Computer Architecture Education(WCAE)
held in conjunction with MICRO-42, pp. 32-39. (December 2009)

[3] ���}���P, �n糐L��, �g������: SimMips: ����E�����ɗL�p��Linux����
��5000�s��MIPS�V�X�e���V�~�����[�^, ��20��R���s���[�^�V�X�e���E�V���|
�W�E��(ComSys2008), pp. 143-150 (November 2008).

[4] �g������, �����^��, �X�J��, ���}���P, �ᐙ�S��, �n糐L��, �A���V,
�X�m��, ���O�c�L��, �������p, ��������, �R�c�T��, �������F, ���їǑ��Y,
�O�D����, ������: MieruPC �v���W�F�N�g: ���g��������v�Z�@�V�X�e����
�\�z���錤���E����v���W�F�N�g, ��20��R���s���[�^�V�X�e���E�V���|�W�E
��(ComSys2008) (November 2008).

[5] �n糐L��, ���}���P, �ᐙ�S��, ���O�c�L��, �X�m��, �g������: MIPS�V
�X�e���V�~�����[�^SimMips �����p�����g���݃V�X�e���J���̌���,�@���
���w����� 2008-EMB-10 (November 2008).

/**********************************************************************/
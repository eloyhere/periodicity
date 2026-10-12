# 周期数学 · Periodic Mathematics

**一个统一描述周期现象的数理框架**
*A Unified Framework of Epicycle Series and Interdisciplinary Periodic Structures*

作者 / Author: **Eloy Kim**（独立研究者 · Independent Researcher）
联系方式 / Contact: `eloy.kim@outlook.com`

[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)
[![Fonts: SIL OFL 1.1](https://img.shields.io/badge/Fonts-SIL%20OFL%201.1-blue.svg)](https://scripts.sil.org/OFL)

> ⚠️ **本作品禁止商业使用。商业使用需获得作者的书面许可。**
> **This work is NOT licensed for commercial use. Commercial use requires a
> separate written licence from the author.** — 详见 [许可](#许可--license)。

---

## 概要 · Overview

本书以「对称 $\to$ 不变量 $\to$ 闭合轨道 $\to$ 周期」为主线，构建了一套从第一原理出发统一周期现象的数学框架。

问题的起点是：不同学科中的周期现象——天体运行、电路振荡、生物节律、经济波动、算法迭代——长期使用互不相通的描述语言。本书主张，它们可以被同一个数学对象刻画：**周期圆族（epicycles）**。一个周期为 $T$ 的系统由振幅、角频率、相位三元组 $(A_k,\omega_k,\phi_k)$ 的集合描述，而周期之间的关系由**周期卷积**刻画。全书由此把"周期性"归结为群作用与群表示的问题：平移、缩放、旋转三种基本变换生成全部初等周期结构，而其完备性可在复平面单参数保角变换族的框架内被严格证明。

全书的核心方法论由四大支柱构成：群表示论、周期卷积、窗函数与折射率诊断、复指数激活函数（CEAF）与核平滑导–积下降法。在此基础上，本书给出把任意目标接入该框架的五步标准接口，并在天文学、物理学、生物学、心理学、经济学、密码学与计算机科学等二十余个领域中逐一施用与验证。

书中严格区分三件事：**已证明的定理**、**以研究纲领形式提出的构想**、以及**尚未解决的问题**。本书被定位为一个**研究纲领**，而非已完成的宏大厦——因此，最有价值的回应是指出它在何处失败，并且尽可能精确。

---

## 版本 · Editions

| 版本 Edition | 语言 Language | 章数 Ch. | 页数 Pages | 源码 Source | 成品 PDF |
|---|---|---|---|---|---|
| British English | 英式英语 | 46 | 1454 | [`Periodicity Mathematics British English/`](<Periodicity Mathematics British English/>) | [PDF](<Periodicity Mathematics British English/Periodicity Mathematics British English.pdf>) |
| Chinese Simplified | 简体中文 | 46 | 1270 | [`Periodicity Mathematics Chinese Simplified/`](<Periodicity Mathematics Chinese Simplified/>) | [PDF](<Periodicity Mathematics Chinese Simplified/Periodicity Mathematics Chinese Simplified.pdf>) |

两个版本共用同一套 46 章的目录结构；中文版另附英文摘要。全书 LaTeX 源码约
95,000 行。

### 内容结构 · Structure

1. **基础与结构** — 运算与周期性、数系、标量与向量、函数与极限、实分析、级数与卷积
2. **分析与几何** — 复分析、四元数分析、微分方程与分析、线性代数、流形与纤维丛、周期流形、周期范畴、概率与分布、向量分析
3. **核心框架** — 周期圆集与均轮球集、传播与演化、群与群函数、群对象与高阶映射、具周期轨道的周期函数与群、周期信号的离散采样与置闰、调和分析、时间序列分析、周期系统
4. **跨学科应用** — 概率、密码、天文、历法、地理、易经、电磁、音频、机械、电路与芯片、生物、心理、社会、气候、经济、政治、数据分析、算法、智能体等系统中的周期结构（约 20 章）
5. **方法与展望** — 周期圆激活函数与函数逼近、从数据到周期圆的建模与分析、总结与展望

---

## 仓库结构 · Repository layout

```
.
├── Periodicity Mathematics British English/    # 英式英语版
│   ├── main.tex                                # 主文件（装配各章）
│   ├── preamble.tex                            # 导言区：宏包、字体、定理环境
│   ├── cover.tex  front.tex  symbol.tex        # 封面、前置、符号表
│   ├── abstract.tex  preface.tex  introduction.tex
│   ├── chapters/                               # 46 章正文
│   ├── back.tex                                # 参考文献与作者简介
│   ├── fonts/                                  # LXGW WenKai（OFL 1.1）
│   ├── compile.bat  clean.bat                  # 编译 / 清理脚本
│   └── Periodicity Mathematics British English.pdf
├── Periodicity Mathematics Chinese Simplified/ # 简体中文版（结构同上）
├── calendar.xlsx                               # 历法数据
├── 周期数学宣讲.pptx                            # 宣讲幻灯片
├── 周期数学·核心理念与应用前景.docx              # 核心理念概述
├── 个人简历-周期数学独立研究者.docx              # 作者简历
├── CITATION.cff                                # 引用信息
├── LICENSE                                     # CC BY-NC-SA 4.0 法律文本
└── LICENSE-SCOPE.md                            # 适用范围与字体例外说明
```

---

## 编译 · Building

需要 **XeLaTeX**（TeX Live 2026 或更新；因使用 `fontspec` 与 `ctex`，不可用 pdfLaTeX）。

```bat
cd "Periodicity Mathematics British English"
compile.bat          :: XeLaTeX 三遍，生成 "Periodicity Mathematics British English.pdf"
clean.bat            :: 清除 .aux/.log/.out/.toc/.lof/.lot/.synctex.gz 等中间文件
```

> **注意**：`compile.bat` 中的 `ENGINE` 变量指向作者本机的 TeX Live 路径
> （`R:\texlive\2026\bin\windows`）。请按需改为你自己的安装路径，或把
> `xelatex` 加入 `PATH` 后直接调用。
>
> `clean.bat` 会**一并删除生成的 PDF**，请留意。

字体已随仓库分发（各版本 `fonts/` 目录，LXGW WenKai / 落霞孤鹜，SIL OFL 1.1），
`preamble.tex` 通过相对路径 `Path = ./fonts/` 加载，因此**克隆后无需额外安装字体即可编译**。

---

## 许可 · License

本项目采用**分许可**结构，请务必区分：

| 内容 | 许可 | 商业使用 |
|---|---|---|
| 书稿源码、PDF、配套文档、本文档 | [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/) | ❌ **禁止**，需另行获得书面许可 |
| `fonts/` 目录下的 LXGW WenKai 字体 | [SIL OFL 1.1](https://scripts.sil.org/OFL) | ✅ 允许（OFL 授权不可被覆盖） |

**在 CC BY-NC-SA 4.0 下**，您可以出于**非商业**目的复制、传播和改编本作品，但必须：
**署名**（注明作者、附许可说明）、**非商业使用**、**相同方式共享**（改编作品须以同一许可发布）。

**商业使用**——包括但不限于出版、转售、纳入付费产品或服务、付费培训或咨询，
或在营利实体的商业业务中使用——**必须事先取得作者的书面授权**。

📧 商业授权洽谈 / Commercial licensing enquiries: **`eloy.kim@outlook.com`**

完整法律文本见 [`LICENSE`](LICENSE)；**适用范围与字体例外的详细说明见
[`LICENSE-SCOPE.md`](LICENSE-SCOPE.md)**（第 A 节：作品；第 B 节：商业使用；
第 C 节：字体例外；第 D 节：无担保）。

---

## 引用 · Citation

引用信息见 [`CITATION.cff`](CITATION.cff)（GitHub 侧栏可直接导出 APA / BibTeX）。
BibTeX：

```bibtex
@book{kim2026periodicity,
  author       = {Kim, Eloy},
  title        = {Periodicity Mathematics: A Unified Framework of Epicycle
                  Series and Interdisciplinary Periodic Structures},
  year         = {2026},
  month        = {9},
  note         = {British English and Chinese Simplified editions},
  publisher    = {Manuscript},
  url          = {https://github.com/eloyhere/periodicity},
  license      = {CC BY-NC-SA 4.0}
}
```

参考文献条目（与 `back.tex` 中 `\bibitem{kim2026}` 一致）：

> **Eloy Kim**. *Periodicity Mathematics: A Unified Framework of Epicycle Series
> and Interdisciplinary Periodic Structures*. Manuscript, 2026.

---

## 反馈 · Feedback

**勘误、反例与反驳尤其欢迎。** 本书以研究纲领的形式提出，最有益的回应是指出它
在何处失败——并且尽可能精确。

Corrections, counterexamples and refutations are especially welcome.

📧 `eloy.kim@outlook.com`

---

*Independent Research in Periodicity Mathematics · September 2026*

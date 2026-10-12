# 许可范围说明 · Licence Scope

本文件说明 [`LICENSE`](LICENSE)（Apache License 2.0）适用于哪些内容、哪些内容例外。
本文件是便利性说明，**不修改也不替代** `LICENSE` 的条款；如有歧义，以 `LICENSE`
的法律文本为准。

This file explains what [`LICENSE`](LICENSE) (Apache License 2.0) covers and what
it does not. It is a convenience note that **does not modify or replace** the
terms of `LICENSE`; in case of ambiguity the legal text governs.

Copyright 2026 Eloy Kim

---

## A. 适用范围 · The Work

以下内容（下称"本作品" / the "Work"）适用 **Apache License 2.0**：

| 内容 | 路径 |
|---|---|
| 两版书稿 LaTeX 源码 | [`Periodicity Mathematics British English/`](<Periodicity Mathematics British English/>)、[`Periodicity Mathematics Chinese Simplified/`](<Periodicity Mathematics Chinese Simplified/>) |
| 两版编译成品 PDF | 各版本的 `*.pdf` |
| 配套文档（幻灯片、核心理念、简历、历法数据） | `*.pptx`、`*.docx`、`*.xlsx` |
| 构建脚本 | `compile.bat`、`clean.bat` |
| 本仓库的说明文档 | `README.md`、`CITATION.cff`、`NOTICE`、本文件 |

**在 Apache License 2.0 下，您可以自由地：**

- **商业使用** —— 用于商业目的、纳入付费产品或服务；
- **修改** —— 改编、翻译、节选、重排；
- **分发** —— 复制、发布、再授权（sublicense）；
- **私用** —— 在不受限制的范围内私人使用。

**唯一需要遵守的条件（第 4 节）：**

1. **附上许可副本** —— 向任何接收者提供一份 `LICENSE`；
2. **标注修改** —— 被修改的文件需带有醒目的"已更改"说明；
3. **保留声明** —— 保留源码中的版权、专利、商标与署名声明；
4. **保留 NOTICE** —— 若分发衍生作品，须一并包含 [`NOTICE`](NOTICE) 中的署名信息。

Apache 2.0 **不要求**衍生作品采用相同许可（无 copyleft），也**不限制**商业使用。

> **请注意**：Apache 2.0 是**宽松许可**。任何人均可免费将本作品用于商业目的，
> **无需**向作者取得额外授权，也**无需**支付费用。若您希望"商业使用需另行取得
> 许可"，则不应使用 Apache 2.0 —— 参见 [`README.md`](README.md) 的许可章节。

Apache 2.0 imposes **no copyleft** and **no non-commercial restriction**. Anyone may
use the Work commercially at no charge, without seeking further permission.

---

## B. 例外：随仓库分发的字体 · Exception: bundled fonts

以下字体文件 **不属于** 本作品，**不受** Apache License 2.0 约束：

The following font files are **not** part of the Work and are **not** covered by
the Apache License 2.0:

```
Periodicity Mathematics British English/fonts/    (6 files)
Periodicity Mathematics Chinese Simplified/fonts/ (6 files)
    LXGWWenKai-Light.ttf   LXGWWenKai-Regular.ttf   LXGWWenKai-Medium.ttf
    LXGWWenKaiMono-Light.ttf  LXGWWenKaiMono-Regular.ttf  LXGWWenKaiMono-Medium.ttf
```

它们属于 **LXGW WenKai（落霞孤鹜）** 字体家族，采用
[**SIL Open Font License 1.1**](https://scripts.sil.org/OFL) 授权。各 `fonts/`
目录下的 `OFL.txt` 为其完整条款。

They are part of the **LXGW WenKai** typeface family by LXGW (落霞孤鹜), licensed
under the [**SIL Open Font License 1.1**](https://scripts.sil.org/OFL). See
`fonts/OFL.txt` in each directory for the full terms.

> Apache 2.0 与本作品的其他条款**不适用于**这些字体文件，也不得被理解为限制
> OFL 所授予的权利。字体仅作为编译书稿所需的资源随仓库分发。
>
> The Apache License 2.0 does not apply to those font files. They are bundled only
> as a build dependency required to compile the books.

---

## C. 无担保 · No warranty

本作品按"现状"提供，不附带任何形式的担保，详见 `LICENSE` 第 7 节与第 8 节。

The Work is provided on an "AS IS" BASIS, without warranties or conditions of any
kind, as set out in Sections 7 and 8 of `LICENSE`.

---

## D. 变更说明 · Change note

本仓库早期修订曾以 **CC BY-NC-SA 4.0（禁止商业使用）** 发布；自本次修订起，
整个仓库改用 **Apache License 2.0**，上述非商业限制**不再适用**。

Earlier revisions of this repository were published under CC BY-NC-SA 4.0, which
prohibited commercial use. As of this revision the repository is licensed under the
Apache License 2.0, and **that non-commercial restriction no longer applies**.

由于版权许可具有**不可撤销性**，已按早期版本获取副本者，其依该版本取得的权利
不受本次变更影响；但**当前版本**的授权条件以 Apache 2.0 为准。

Because copyright grants are **irrevocable**, recipients who obtained a copy under an
earlier revision retain the rights granted to them by it; the **current revision**,
however, is governed by Apache 2.0.

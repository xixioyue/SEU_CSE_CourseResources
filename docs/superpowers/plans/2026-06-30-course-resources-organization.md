# Course Resources Organization Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Reorganize all course resources into confirmed freshman and sophomore course directories, exclude exactly three oversized files from Git, and publish the result through a Draft PR.

**Architecture:** Align `codex/organize-course-resources` to `origin/main` while preserving the current working-tree files, then perform an in-place, one-to-one directory move. The current local course folders, especially the modified `计组` folder, are authoritative and replace the remote course files. Add only a root course index and exact ignore rules, then validate paths, content digests, file counts, and Git size limits before committing and pushing.

**Tech Stack:** PowerShell, Git, GitHub

---

### Task 1: Verify the Local and Remote Baseline

**Files:**
- Read: `docs/superpowers/specs/2026-06-30-course-resources-organization-design.md`
- Read: `.git/config`

- [ ] **Step 1: Confirm the active branch and target remote**

Run:

```powershell
git branch --show-current
$origin = git remote get-url origin
if ($origin -notmatch '(^|[:/])xixioyue/SEU_CSE_CourseResources(\.git)?$') { throw 'origin does not match target repository' }
```

Expected: branch is `codex/organize-course-resources`; no exception is raised.

- [ ] **Step 2: Fetch the current remote base**

Run:

```powershell
git fetch origin main
```

Expected: `origin/main` is fetched successfully.

- [ ] **Step 3: Capture the authoritative local 计组 digest**

Run:

```powershell
$localCompositionFiles = Get-ChildItem -LiteralPath '计组' -File -Recurse | Sort-Object FullName
$localCompositionBase = (Resolve-Path -LiteralPath '计组').Path
$localCompositionEntries = foreach ($file in $localCompositionFiles) {
  $relative = $file.FullName.Substring($localCompositionBase.Length).TrimStart([char]'\')
  $hash = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash
  "$relative|$($file.Length)|$hash"
}
$localCompositionBytes = ($localCompositionFiles | Measure-Object Length -Sum).Sum
$digestBytes = [Text.Encoding]::UTF8.GetBytes(($localCompositionEntries -join "`n"))
$sha256 = [Security.Cryptography.SHA256]::Create()
$localCompositionDigest = ([BitConverter]::ToString($sha256.ComputeHash($digestBytes))).Replace('-','')
$localCompositionSnapshot = [pscustomobject]@{
  FileCount = $localCompositionFiles.Count
  TotalBytes = $localCompositionBytes
  Digest = $localCompositionDigest
}
$localCompositionSnapshot
```

Expected:

```text
FileCount: 424
TotalBytes: 560302279
Digest: E690747930A761902E48713FB530A8A6C578A0B5EE8CEBFB185EC757A2DD0A77
```

- [ ] **Step 4: Align the branch to origin/main without changing working-tree files**

Run:

```powershell
git reset --mixed origin/main
if ((git rev-parse HEAD) -ne (git rev-parse origin/main)) { throw 'branch did not align to origin/main' }
```

Expected: `HEAD` equals `origin/main`; current working-tree course files remain present.

- [ ] **Step 5: Verify the authoritative local 计组 digest is unchanged**

Run:

```powershell
$base = (Resolve-Path -LiteralPath '计组').Path
$files = Get-ChildItem -LiteralPath '计组' -File -Recurse | Sort-Object FullName
$entries = foreach ($file in $files) {
  $relative = $file.FullName.Substring($base.Length).TrimStart([char]'\')
  $hash = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash
  "$relative|$($file.Length)|$hash"
}
$bytes = ($files | Measure-Object Length -Sum).Sum
$digestBytes = [Text.Encoding]::UTF8.GetBytes(($entries -join "`n"))
$sha256 = [Security.Cryptography.SHA256]::Create()
$digest = ([BitConverter]::ToString($sha256.ComputeHash($digestBytes))).Replace('-','')
if ($files.Count -ne 424) { throw '计组 file count changed during baseline alignment' }
if ($bytes -ne 560302279) { throw '计组 byte count changed during baseline alignment' }
if ($digest -ne 'E690747930A761902E48713FB530A8A6C578A0B5EE8CEBFB185EC757A2DD0A77') { throw '计组 content digest changed during baseline alignment' }
```

Expected: no exception is raised.

- [ ] **Step 6: Confirm every source exists and every destination is absent**

Run a PowerShell preflight over the exact mapping:

```powershell
$mapping = [ordered]@{
  '工数' = '大一\工科数学分析'
  '工数实验' = '大一\工科数学分析实验'
  '数电' = '大一\数字逻辑电路'
  '线代' = '大一\线性代数'
  '雅思' = '大一\雅思英语'
  'OS' = '大二\操作系统'
  '大物下' = '大二\大学物理（下）'
  '概率论' = '大二\概率论与数理统计'
  '计组' = '大二\计算机组成原理'
  '计组实践' = '大二\计算机组成原理专题实践'
  '毛概' = '大二\毛概'
  '数据结构' = '大二\数据结构'
  '算法设计与分析' = '大二\算法设计与分析'
  '习概' = '大二\习概'
  '信号与系统' = '大二\信号与系统'
}
$root = (Get-Location).Path
foreach ($entry in $mapping.GetEnumerator()) {
  $source = Join-Path $root $entry.Key
  $target = Join-Path $root $entry.Value
  if (-not (Test-Path -LiteralPath $source -PathType Container)) { throw "missing source: $source" }
  if (Test-Path -LiteralPath $target) { throw "target already exists: $target" }
  if (-not [IO.Path]::GetFullPath($target).StartsWith($root + [IO.Path]::DirectorySeparatorChar)) { throw "target escapes workspace: $target" }
}
```

Expected: no exception is raised.

### Task 2: Move and Rename Course Directories

**Files:**
- Move: `工数` to `大一/工科数学分析`
- Move: `工数实验` to `大一/工科数学分析实验`
- Move: `数电` to `大一/数字逻辑电路`
- Move: `线代` to `大一/线性代数`
- Move: `雅思` to `大一/雅思英语`
- Move: `OS` to `大二/操作系统`
- Move: `大物下` to `大二/大学物理（下）`
- Move: `概率论` to `大二/概率论与数理统计`
- Move: `计组` to `大二/计算机组成原理`
- Move: `计组实践` to `大二/计算机组成原理专题实践`
- Move: `毛概` to `大二/毛概`
- Move: `数据结构` to `大二/数据结构`
- Move: `算法设计与分析` to `大二/算法设计与分析`
- Move: `习概` to `大二/习概`
- Move: `信号与系统` to `大二/信号与系统`

- [ ] **Step 1: Capture source file counts**

Run:

```powershell
$mapping = [ordered]@{
  '工数' = '大一\工科数学分析'
  '工数实验' = '大一\工科数学分析实验'
  '数电' = '大一\数字逻辑电路'
  '线代' = '大一\线性代数'
  '雅思' = '大一\雅思英语'
  'OS' = '大二\操作系统'
  '大物下' = '大二\大学物理（下）'
  '概率论' = '大二\概率论与数理统计'
  '计组' = '大二\计算机组成原理'
  '计组实践' = '大二\计算机组成原理专题实践'
  '毛概' = '大二\毛概'
  '数据结构' = '大二\数据结构'
  '算法设计与分析' = '大二\算法设计与分析'
  '习概' = '大二\习概'
  '信号与系统' = '大二\信号与系统'
}
$sourceCounts = @{}
foreach ($entry in $mapping.GetEnumerator()) {
  $sourceCounts[$entry.Key] = @(Get-ChildItem -LiteralPath $entry.Key -File -Recurse).Count
}
```

Expected: 15 count entries are captured.

- [ ] **Step 2: Create the year directories**

Run:

```powershell
New-Item -ItemType Directory -Path '大一','大二' -ErrorAction Stop
```

Expected: both directories are created.

- [ ] **Step 3: Move every course with literal paths**

Run:

```powershell
foreach ($entry in $mapping.GetEnumerator()) {
  Move-Item -LiteralPath $entry.Key -Destination $entry.Value -ErrorAction Stop
}
```

Expected: all 15 source directories move successfully.

- [ ] **Step 4: Verify every move preserved its file count**

Run:

```powershell
foreach ($entry in $mapping.GetEnumerator()) {
  if (Test-Path -LiteralPath $entry.Key) { throw "source remains: $($entry.Key)" }
  $targetCount = @(Get-ChildItem -LiteralPath $entry.Value -File -Recurse).Count
  if ($targetCount -ne $sourceCounts[$entry.Key]) {
    throw "file count mismatch: $($entry.Key) expected=$($sourceCounts[$entry.Key]) actual=$targetCount"
  }
}
```

Expected: no exception is raised.

### Task 3: Add Repository Index and Exact Ignore Rules

**Files:**
- Create: `.gitignore`
- Create: `README.md`

- [ ] **Step 1: Add exact oversized-file ignore rules**

Create `.gitignore` with exactly:

```gitignore
/大二/算法设计与分析/算法导论（原书第3版）_13234228.pdf
/大二/计算机组成原理/课程资料/深入理解计算机系统原书第三版.pdf
/大二/数据结构/数据结构基础（C++语言版）第2版.pdf
```

- [ ] **Step 2: Add the course index**

Create `README.md` with:

```markdown
# SEU CSE Course Resources

东南大学计算机相关课程资料，按学年和课程整理。

## 大一

- [工科数学分析](大一/工科数学分析)
- [工科数学分析实验](大一/工科数学分析实验)
- [数字逻辑电路](大一/数字逻辑电路)
- [线性代数](大一/线性代数)
- [雅思英语](大一/雅思英语)

## 大二

- [操作系统](大二/操作系统)
- [大学物理（下）](大二/大学物理（下）)
- [概率论与数理统计](大二/概率论与数理统计)
- [计算机组成原理](大二/计算机组成原理)
- [计算机组成原理专题实践](大二/计算机组成原理专题实践)
- [毛概](大二/毛概)
- [数据结构](大二/数据结构)
- [算法设计与分析](大二/算法设计与分析)
- [习概](大二/习概)
- [信号与系统](大二/信号与系统)
```

### Task 4: Validate the Organized Repository

**Files:**
- Verify: `.gitignore`
- Verify: `README.md`
- Verify: `大一/**`
- Verify: `大二/**`

- [ ] **Step 1: Verify the expected course directories**

Run:

```powershell
$actualFreshman = Get-ChildItem -LiteralPath '大一' -Directory | Sort-Object Name | Select-Object -ExpandProperty Name
$actualSophomore = Get-ChildItem -LiteralPath '大二' -Directory | Sort-Object Name | Select-Object -ExpandProperty Name
if ($actualFreshman.Count -ne 5) { throw "unexpected freshman course count: $($actualFreshman.Count)" }
if ($actualSophomore.Count -ne 10) { throw "unexpected sophomore course count: $($actualSophomore.Count)" }
```

Expected: no exception is raised.

- [ ] **Step 2: Verify all oversized files still exist and are ignored**

Run:

```powershell
$oversized = @(
  '大二/算法设计与分析/算法导论（原书第3版）_13234228.pdf',
  '大二/计算机组成原理/课程资料/深入理解计算机系统原书第三版.pdf',
  '大二/数据结构/数据结构基础（C++语言版）第2版.pdf'
)
foreach ($path in $oversized) {
  if (-not (Test-Path -LiteralPath $path -PathType Leaf)) { throw "missing oversized file: $path" }
  git check-ignore -q -- $path
  if ($LASTEXITCODE -ne 0) { throw "oversized file is not ignored: $path" }
}
```

Expected: all three files exist and are ignored.

- [ ] **Step 3: Verify no upload candidate exceeds 100 MB**

Run:

```powershell
$oversized = @(
  '大二/算法设计与分析/算法导论（原书第3版）_13234228.pdf',
  '大二/计算机组成原理/课程资料/深入理解计算机系统原书第三版.pdf',
  '大二/数据结构/数据结构基础（C++语言版）第2版.pdf'
)
$tooLarge = Get-ChildItem -LiteralPath '大一','大二' -File -Recurse |
  Where-Object { $_.Length -gt 100MB -and $_.FullName -notin ($oversized | ForEach-Object { (Resolve-Path -LiteralPath $_).Path }) }
if ($tooLarge) { $tooLarge | Select-Object Length,FullName; throw 'unignored files exceed 100 MB' }
```

Expected: no files are printed and no exception is raised.

- [ ] **Step 4: Validate generated text files and review Git changes**

Run:

```powershell
git diff --check
git status --short
```

Expected: no whitespace errors; status contains only the planned documentation, ignore rules, course moves, additions, and deletions.

### Task 5: Commit the Organized Resources

**Files:**
- Stage: `.gitignore`
- Stage: `README.md`
- Stage: `大一/**`
- Stage: `大二/**`
- Stage deletions from the former root course paths
- Stage: `docs/superpowers/plans/2026-06-30-course-resources-organization.md`

- [ ] **Step 1: Stage the complete planned change**

Run:

```powershell
git add -A
```

Expected: all planned moves, additions, deletions, and documentation are staged; ignored oversized files remain unstaged.

- [ ] **Step 2: Verify the staged change**

Run:

```powershell
git diff --cached --check
git status --short
git diff --cached --stat
```

Expected: no whitespace errors and no oversized ignored file appears as staged content.

- [ ] **Step 3: Commit the organized resources**

Run:

```powershell
git commit -m "整理大一大二课程资源目录"
```

Expected: one new commit is created on `codex/organize-course-resources`.

### Task 6: Push and Open a Draft PR

**Files:**
- No local file changes

- [ ] **Step 1: Push the branch**

Run:

```powershell
git push -u origin codex/organize-course-resources
```

Expected: the remote branch is created and upstream tracking is configured.

- [ ] **Step 2: Create the Draft PR**

Create a Draft PR targeting `main` with:

```text
Title: 整理大一大二课程资源目录

Body:
## 变更

- 按大一、大二重新组织课程目录
- 将课程简称补全为确认后的名称
- 保留课程目录内部结构和文件名
- 新增课程索引和大文件忽略规则

## 验证

- 已核对 15 门课程的目录映射
- 已核对移动前后的文件数量
- 已确认待上传文件均不超过 GitHub 单文件限制
```

Expected: a Draft PR is opened from `codex/organize-course-resources` to `main`.

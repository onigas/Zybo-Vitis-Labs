# Apply the Update and Publish to Your Own GitHub Repository

[Home](../../README.md)

This guide starts with the repository already on your computer and visible
in GitHub Desktop. The package is **not a complete upstream clone**; it is an
update to copy into that clone's root. It contains no `.git` directory and
does not modify your remote repository automatically.

## 1. Preserve your current work

In GitHub Desktop, select `Zynq-Design-using-Vivado` and inspect **Changes**.
Commit your own uncommitted changes or make a separate backup first.
Do not overwrite a `lab*.md` file that you have edited without reviewing it.

Use **Current Branch > New Branch** or **Branch > New Branch** to create a
branch such as **`vitis-2022.2-zybo`**.
This lets you preserve the original branch unchanged.

## 2. Copy the package

**Repository > Show in Explorer** opens the local repository root, where the
old `README.md` and `lab1.md` through `lab5.md` are located.

Extract the ZIP into a separate directory and open its **`overlay`** directory.
Copy **the CONTENTS of `overlay`** into the repository root; do not create an
additional `overlay` subdirectory inside the repository.

These six existing documents are intentionally replaced:

```text
README.md
lab1.md
lab2.md
lab3.md
lab4.md
lab5.md
```

The `sources/vitis2022_2`, `scripts/vitis2022_2`, `docs/vitis2022_2` and
`tests/vitis2022_2` directories provide the adaptation's additional content.
If they already contain your own work, compare the files before replacing them.

You **do not need to delete** the original `sources/lab*`, `labsolutions`,
`pics`, `slides` or `board_files` content. These are not rebuilt solutions for
the new Vitis version.

As needed, **append** `docs/vitis2022_2/gitignore.append.txt` to your existing
`.gitignore`; do not replace existing rules. Keeping the working directory
outside the repository reduces accidental commits of generated files.
A `.gitignore` rule does not automatically stop tracking files that were
already committed.

### Replacing the earlier Hungarian package

If you already applied the earlier package, copy this English overlay over it.
The lab filenames remain `lab1.md` through `lab5.md`, so those files are
replaced in place. The supporting guides now use English, unsuffixed names.

After preserving any edits you made, remove only these obsolete files from
`docs/vitis2022_2` if they came from the earlier package:

```text
GITHUB_DESKTOP_HU.md
MIGRATION_NOTES_HU.md
SETUP_HU.md
TEST_STATUS_HU.md
VITIS_WORKFLOW_HU.md
```

Their replacements are `GITHUB_DESKTOP.md`, `MIGRATION_NOTES.md`, `SETUP.md`,
`TEST_STATUS.md` and `VITIS_WORKFLOW.md`. Check your own extra documents for
links to the old filenames. Do not delete unrelated files.

## 3. Review and commit

Return to **Changes** in GitHub Desktop and review the diff.
Expect the six Markdown replacements and the Vitis-specific directories.
Review any additional changes of your own separately.

Do not commit a complete Vitis `.metadata` directory, multi-gigabyte Vivado
build outputs or an accidental second `.git` directory.
Hand-edited C/HDL/XDC/Tcl, packaged custom IP and final linker scripts are
valuable source material to preserve.

Example commit message for the initial adaptation:

```text
Add Zybo legacy labs for Vivado and Vitis 2022.2
```

For this language correction after committing the earlier package, use:

```text
Use English lab instructions and supporting documentation
```

Select **Commit to vitis-2022.2-zybo**.

## 4. Publish to your own account, not to XUP's repository

A local clone is not necessarily connected to your own GitHub repository.
Check the remote address under **Repository > Repository settings > Remote**.
If it points to `xupgit/Zynq-Design-using-Vivado`, you cloned the upstream
repository.

GitHub Desktop supports creating a fork when you try to publish to a repository
for which you do not have write access. After **Publish branch / Push origin**,
it may offer to create a fork. For your own educational version, choose
**For my own purposes** and verify that the resulting fork belongs to your
account.

Alternatively, first create a fork on the GitHub website and clone it using
GitHub Desktop. In that case, copy this update into **your fork's local
directory**, not the separate upstream clone.

You do not need to force-push, delete `.git`, or overwrite an upstream branch.
Using a fork rather than an unrelated new repository preserves the origin
and version history.

## 5. Check the published result

In your browser, open the repository under **your own username** and select
the `vitis-2022.2-zybo` branch. Check the README's lab links and the new source
files.

To show this version by default on GitHub, you can later merge the branch into
your fork's default branch or change the default branch. Complete the
[hardware validation checklist](TEST_STATUS.md) before treating the adaptation
as verified.

Preserve upstream attribution and check the original material's distribution
terms. This package does not assign a new uniform license to every file in
the original repository.

Official fork, clone and publish documentation:
[GitHub Desktop documentation](https://docs.github.com/desktop/adding-and-cloning-repositories/cloning-and-forking-repositories-from-github-desktop).

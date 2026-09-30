import os
import shutil

print(">>> Running copy_uf2.py as post script <<<")

Import("env")

def after_build(source, target, env):
    print(">>> Inside after_build function <<<")
    env_name = env["PIOENV"]
    build_dir = env.subst("$BUILD_DIR")
    src = os.path.join(build_dir, "firmware.uf2")
    if os.path.exists(src):
        out_dir = os.path.join(env["PROJECT_DIR"], "build")
        if not os.path.exists(out_dir):
            os.makedirs(out_dir)
        dst = os.path.join(out_dir, f"{env_name}.uf2")
        shutil.copyfile(src, dst)
        print(f"Copied {src} to {dst}")
    else:
        print(f"Source file not found: {src}")

env.AddPostAction("buildprog", after_build)
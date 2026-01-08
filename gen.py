import sys
import trimesh

def r(a):
    return round(a, 6)

def wVec3(f, name, cont):
    f.write(f"const vec3 {name}[] = {{\n")
    for v in cont:
        f.write(f"    {{{r(v[0])}, {r(v[1])}, {r(v[2])}}},\n")
    f.write("};\n\n")


def wInd(f, name, cont):
    f.write(f"const size_t {name}[] = {{\n")
    for tri in cont:
        f.write(f"    {tri[0]}, {tri[1]}, {tri[2]},\n")
    f.write("};\n\n")

mesh = trimesh.load(sys.argv[1], force="mesh")

if (mesh.vertex_normals is None) or len(mesh.vertex_normals) == 0:
    mesh.rezero()
    mesh.fix_normals()

print("info")
print(f"-v: {len(mesh.vertices)}")
print(f"-f: {len(mesh.faces)}")

faces = [len(face) for face in mesh.faces]
mn = min(faces)
mx = max(faces)

print(f"-v/f: mn={mn}, mx={mx}")

if mn == 3 and mx == 3:
    print("-type: tri")
else:
    print("-type: idk)")

with open("model.h", "w") as f:
    w = f"#pragma once\n#include \"types.h\"\nconst int modelType = 3;\n//{sys.argv[1]}\n"
    f.write(w)

    wVec3(f, "vertices", mesh.vertices)
    wInd(f, "indices", mesh.faces)
    wVec3(f, "normals", mesh.vertex_normals)

print("\nmodel.h")

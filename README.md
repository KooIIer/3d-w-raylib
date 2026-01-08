https://casual-effects.com/data/<br>
<br>
./comp.sh - compile<br>
./run.sh args - comple + run<br>
python gen.py file.obj- gen model.h<br>
./model.sh file.obj args - gen model.h + compile + run<br>
<br>
args:<br>
debug<br>
dist ...<br>
culling (//TODO fix)<br>
transparent<br>
<br>
<br>
<br>
windowrule = no_blur on, match:class ^raylib$<br>
windowrule = no_shadow on, match:class ^raylib$<br>
windowrule = border_size 0, match:class ^raylib$<br>
windowrule = rounding 0, match:class ^raylib$<br>
windowrule = decorate off, match:class ^raylib$<br>

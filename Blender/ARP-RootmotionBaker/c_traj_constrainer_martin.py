import bpy

def button_callback(self, context):

    obj =  bpy.data.scenes["Scene"].target
    
    if obj is None:
        return
    if obj.type != 'ARMATURE':
        return
    # Reparenting
    bpy.ops.object.mode_set(mode='EDIT')
    obj.data.edit_bones["c_root_master.x"].parent = obj.data.edit_bones["c_pos"]

    traj_control = obj.pose.bones["c_traj"]
    if traj_control is None:
        return
    
    # Location Constraints
    location = traj_control.constraints.new('COPY_LOCATION')
    location.target = obj
    location.subtarget = "c_root_master.x"
    location.use_x = True
    location.use_y = True
    location.use_z = False

    # Rotation Constraints
    rotation = traj_control.constraints.new('COPY_ROTATION')
    rotation.target = obj
    rotation.subtarget = "c_root_master.x"
    rotation.use_x = False
    rotation.use_y = False
    rotation.use_z = True

    controller_names = [
    "c_foot_ik.l",
    "c_foot_ik.r",
    "c_hand_ik.l",
    "c_hand_ik.r",
    "c_leg_pole.l",
    "c_leg_pole.r",
    "head_scale_fix.x"
    ]
    constraint_names = [
    "Child Of",
    "Child Of_global",
    "Child Of_traj"
    ]
  
    # Change constraints from c_traj
    for i in  controller_names:
        for j in  constraint_names:
            try:
                obj.pose.bones[i].constraints[j].subtarget = "c_pos"
            except:
                pass
    bpy.ops.object.mode_set(mode='OBJECT')

class CustomButtonOperator(bpy.types.Operator):
    bl_idname = "object.custom_button_operator"
    bl_label = "Create Constraints"

    def execute(self, context):
        button_callback(self, context)
        return {'FINISHED'}

class CustomPanel(bpy.types.Panel):
    bl_idname = "OBJECT_PT_custom_panel"
    bl_label = "Dimfrost Tools"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'Martins Custom'

    def draw(self, context):
        self.layout.label(text= "ARP - Setup c_traj Constraints")
        self.layout.prop_search(context.scene, "target", context.scene, "objects", text="Rig")
        self.layout.operator("object.custom_button_operator")

def register():
    bpy.utils.register_class(CustomButtonOperator)
    bpy.utils.register_class(CustomPanel)
    bpy.types.Scene.target = bpy.props.PointerProperty(type=bpy.types.Armature, poll=p_filter)

def unregister():
    bpy.utils.unregister_class(CustomButtonOperator)
    bpy.utils.unregister_class(CustomPanel)
    del bpy.types.Scene.target

def p_filter(self, object):
    return object.type == 'ARMATURE' #and object.visible_get()

if __name__ == "__main__":
    register()
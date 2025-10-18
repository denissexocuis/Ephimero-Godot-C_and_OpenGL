extends Node2D

@onready var animated_sprite = $AnimatedSprite2D
@onready var label: Label = $ESPACIO

func _ready():
	animated_sprite.animation = "animation"
	animated_sprite.play()

func _on_area_2d_body_entered(body: Node2D) -> void:
	label.show()

func _on_area_2d_body_exited(body: Node2D) -> void:
	label.hide()

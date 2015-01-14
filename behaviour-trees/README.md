# jbt

A minimalistic, metaprogammed behaviour-tree library for C.

The idea is to take a textual representation of a BT, such as:

    brain robot {
        repeat {
            select {
                sequence {
                    is_droid_in_range()
                    select {
                        sequence {
                            droid_is_weaker()
                            attack_droid()
                        }
                        select {
                            sequence {
                                find_safe_tile()
                                move_to()
                            }
                            attack_droid()
                        }
                    }
                }
                wander() {
                    move_to()
                }
            }
        }       
    }

And convert it into efficent C code (example adapted from the `obviam.net` article, referenced below).

## Thoughts

Brains should be able to have arbitrary numbers of extra stacks.

## References

  - http://www.gamasutra.com/blogs/ChrisSimpson/20140717/221339/Behavior_trees_for_AI_How_they_work.php
  - http://en.wikipedia.org/wiki/Behavior_Trees_(Artificial_Intelligence,_Robotics_and_Control)
  - http://obviam.net/index.php/game-ai-an-introduction-to-behavior-trees/
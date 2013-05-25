/*
 * TODO:
 * - use a layout to put rectangles on the stage, rather than absolute positioning
 * - fix file searchPath so it works when run from anywhere
 */

// X is always the first player
// NB relies on a patched gjs with a 'print' function (used to show the winner)

// import local file - NB this means the script only works from this directory at the moment
imports.searchPath.push('.');
const Ox = imports.board;

const Clutter = imports.gi.Clutter;
const Lang = imports.lang;

const OX_PLAYERS = ["X", "O"];
const OX_SQUARE_SIZE_PX = 100;
const OX_FONT_SIZE = OX_SQUARE_SIZE_PX * 0.95;
const OX_LINE_WIDTH_PX = 10;
const OX_SIDE_SIZE = 3;
const OX_TEXT_COLOR = new Clutter.Color( {'red':255, 'blue':255, 'green':255, 'alpha':255} );
const OX_STRIKE_COLOR = new Clutter.Color( {'red':255, 'blue':0, 'green':0, 'alpha':255} );

function BoardView(players, sideSize) {
    this._init(players, sideSize);
};

BoardView.prototype = {
    _init : function (players, sideSize) {
        this.board = new Ox.Board(players, sideSize);

        this.boardSizePx = (sideSize * OX_SQUARE_SIZE_PX) + (OX_LINE_WIDTH_PX * (sideSize + 1));

        Clutter.init (0, null);

        this.stage = Clutter.Stage.get_default();
        this.stage.title = "ox"
        this.stage.set_size(this.boardSizePx, this.boardSizePx);

        let c = new Clutter.Color( {'red':50, 'blue':50, 'green':50, 'alpha':255} );

        for (let i = 0; i < sideSize; i++) {
            for (let j = 0; j < sideSize; j++) {
                let xpos = ((i + 1) * OX_LINE_WIDTH_PX) + (i * OX_SQUARE_SIZE_PX);
                let ypos = ((j + 1) * OX_LINE_WIDTH_PX) + (j * OX_SQUARE_SIZE_PX);

                let r = new Clutter.Rectangle();
                r.set_size(OX_SQUARE_SIZE_PX, OX_SQUARE_SIZE_PX);
                r.set_color(c);
                r.set_position(xpos, ypos);
                r.set_reactive(true);

                let x = i;
                let y = j;

                r.connect('button-press-event', Lang.bind(this, function(actor, event) {
                    if (this.board.canMove(x, y)) {
                        let nextPlayer = this.board.getNextPlayer();

                        let line = this.board.makeMove(x, y, nextPlayer);

                        this.markMove(r, nextPlayer);

                        if (line.winsFor()) {
                            this.strikeThrough(line);
                        }

                        this.board.switchPlayer();
                    }
                }));

                this.stage.add_actor(r);
            }
        }
    },

    strikeThrough : function (line) {
        let first = line.first();
        let last = line.last();

        let height = 0;
        let width = 0;
        let x = 0;
        let y = 0;
        let rotate = 0;
        let straight_line_length = this.boardSizePx * 0.95;

        if (first.getX() == last.getX()) {
            // column
            width = OX_LINE_WIDTH_PX  / 2;
            height = straight_line_length;
            x = ((first.getX() + 0.5) * OX_SQUARE_SIZE_PX) + ((first.getX() + 0.75) * OX_LINE_WIDTH_PX);
            y = (this.boardSizePx - straight_line_length) / 2;
        }
        else if (first.getY() == last.getY()) {
            // row
            width = straight_line_length;
            height = OX_LINE_WIDTH_PX / 2;
            x = (this.boardSizePx - straight_line_length) / 2;
            y = ((first.getY() + 0.5) * OX_SQUARE_SIZE_PX) + ((first.getY() + 0.75) * OX_LINE_WIDTH_PX);
        }
        else {
            // diagonal
            width = Math.sqrt(straight_line_length * straight_line_length * 2);
            height = OX_LINE_WIDTH_PX / 2;
            x = (this.boardSizePx - width) / 2;
            y = (this.boardSizePx / 2) - (height / 2);

            if (first.getX() == first.getY()) {
                rotate = 45;
            }
            else {
                rotate = -45;
            }
        }

        let strike = new Clutter.Rectangle ();
        strike.set_color (OX_STRIKE_COLOR);
        strike.set_position (x, y);
        strike.set_size (width, height);

        if (rotate != 0) {
            strike.set_rotation (Clutter.RotateAxis.Z_AXIS, rotate, width / 2, height / 2, 0);
        }

        this.stage.add_actor(strike);
    },

    markMove : function(clickedRectangle, player) {
        let t = new Clutter.Text( {"text":player, "color":OX_TEXT_COLOR} );

        t.set_font_name("Sans Bold " + OX_FONT_SIZE + "px");

        let [r_x, r_y] = clickedRectangle.get_position();
        let offset_x = (clickedRectangle.get_width() / 2) - (t.get_width() / 2); 
        let offset_y = (clickedRectangle.get_height() / 2) - (t.get_height() / 2);

        t.set_position(r_x + offset_x, r_y + offset_y);
        t.move_anchor_point_from_gravity (Clutter.Gravity.CENTER);

        this.stage.add_actor(t);

        let timeline = new Clutter.Timeline( {'duration':500} );
        let alpha = new Clutter.Alpha ( {'timeline':timeline, 'mode':Clutter.AnimationMode.EASE_OUT_ELASTIC} );
        let behave = new Clutter.BehaviourScale ( {'alpha':alpha, 'x_scale_start':1.0, 'y_scale_start':1.0, 'x_scale_end':0.5, 'y_scale_end':0.5} );
        behave.apply (t);
        timeline.start ();
    },

    show : function() {
        this.stage.show();
        Clutter.main();
        this.stage.destroy();
    }
};


let view = new BoardView(OX_PLAYERS, OX_SIDE_SIZE);
view.show();

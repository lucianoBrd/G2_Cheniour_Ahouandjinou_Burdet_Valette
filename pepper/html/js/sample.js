$(document).ready(function () {
    session = new QiSession();

    session.service("ALMemory").done(function(ALMemory) {

        ALMemory.subscriber("Home/Label").done(function(subscriber) {

            subscriber.signal.connect(function(data) {
                $('.home').html(data);
            });
        });

    });

    function raise(event, value) {
        session.service("ALMemory").done(function(ALMemory) {
            ALMemory.raiseEvent(event, value);
        });
    }

});

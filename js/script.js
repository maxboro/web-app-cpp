document.addEventListener("DOMContentLoaded", function () {
    let counterFrontend = 0;
    const counterElement = document.getElementById("counter-frontend");

    document.getElementById("incr_count").addEventListener("click", function () {
        counterFrontend++;
        counterElement.textContent = counterFrontend;
    });
});

<script lang="ts">
    interface Contact{
        telephone_numbers : String[];
        emails : String[];
    }
   export let contact : Contact = {telephone_numbers: [], emails : []};


</script>

<div>
    {#each contact.telephone_numbers as number}
        <p>{number}</p>
    {/each}
    {#each contact.emails as mail}
        <p>{mail}</p>
    {/each}
</div>
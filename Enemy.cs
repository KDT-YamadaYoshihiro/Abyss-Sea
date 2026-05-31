using UnityEngine;

public class Enemy : MonoBehaviour
{
    public Image enemyImage;

    public void FadeOut()
    {
        StartCoroutine(FadeOutAnimation());
    }

    IEnumerator FadeOutAnimation()
    {
        float alpha = 1.0f;
        while (alpha > 0.0f)
        {
            alpha -= Time.deltaTime * 0.1f;
            enemyImage.color = new Color(enemyImage.color.r, enemyImage.color.g, enemyImage.color.b, alpha);
            yield return null;
        }
        Destroy(gameObject);
    }
}